#include "bullets.hpp"

Bullet::Bullet(Point _position, int _v, double _rotation, int _lifetime)
{
    position = _position;
    lifetime = _lifetime;
    v_x = (double)_v * cos(_rotation * PI / PIANGLE);
    v_y = (double)_v * sin(_rotation * PI / PIANGLE);
    has_shield = false;
}

Gift::Gift(Point _position, int t) 
{
    position = Point(_position.x * TANK_SIZE + HALF_TANK_SIZE, _position.y * TANK_SIZE + HALF_TANK_SIZE);
    
    type = Tank_Types[t];
    
    tank_collision = false;
    
    filename = Assets_Dir + SLASH + type + DASH + Generic_Pickup + Extension;
}

Gift::Gift(string _type, int _count, int _time)
: type(_type), 
  shoot_count(_count), 
  lifetime(_time) 
{

}

Bullets_Repository::Bullets_Repository(int tank_count, Map* _map, Window* _win) 
{
    tank_bullets = vector < vector <Bullet*>> (tank_count);
    map = _map;
    game_window = _win;
}

void Bullets_Repository::add_to_bullets(int tank_id, Point _position, double _rotation) 
{
    if(tank_bullets[tank_id - 1].size() == 5) 
        return;

    _position += Point((HALF_TANK_SIZE + 5) * cos(_rotation * PI / 180), (HALF_TANK_SIZE + 5) * sin(_rotation * PI / 180));
    
    Bullet* b = new Bullet(_position, BULLET_VELOCITY, _rotation, BULLET_LIFETIME);
    
    check_bullet_wall_collision(b);
    
    tank_bullets[tank_id - 1].push_back(b);
    
    game_window->play_sound_effect(Assets_Dir + SLASH + Tank_Shot + Wav);
}

void Bullets_Repository::add_to_pickup_bullets(Point _position, double _rotation, string gif) 
{
    if(gif == SHOTGUN) 
    {
        vector<Bullet*> b(5);        
        vector<Point> p(5);
        vector<double> r(5);

        _rotation -= 20.00;
        
        for(int i = 0; i < 5; i++) 
        {
            r[i] += _rotation + i * 10.00;
            p[i] = _position + Point((HALF_TANK_SIZE + 10) * cos(r[i] * PI / PIANGLE), (HALF_TANK_SIZE) * sin(r[i] * PI / PIANGLE));
            b[i] = new Bullet(p[i], PICKUP_BULLET_VELOCITY, r[i], DOUBLEBARREL_LIFETIME);

            check_bullet_wall_collision(b[i]);

            pickup_bullets.push_back(b[i]);
        }
    }

    else if(gif == DOUBLEBARREL) 
    {
        _position += Point((HALF_TANK_SIZE + 5) * cos(_rotation * PI / PIANGLE), (HALF_TANK_SIZE + 5) * sin(_rotation * PI / PIANGLE));

        double new_rotation1 = _rotation + RIGHTANGLE;
        double new_rotation2 = _rotation - RIGHTANGLE;
        
        Point bullet_pos1 = _position + Point((BULLET_SIZE) * cos(new_rotation1 * PI / PIANGLE), BULLET_SIZE * sin(new_rotation1 * PI / PIANGLE));
        Point bullet_pos2 = _position + Point((BULLET_SIZE) * cos(new_rotation2 * PI / PIANGLE), BULLET_SIZE * sin(new_rotation2 * PI / PIANGLE));
        
        Bullet* bullet1 = new Bullet(bullet_pos1, BULLET_VELOCITY, _rotation, DOUBLEBARREL_LIFETIME);
        Bullet* bullet2 = new Bullet(bullet_pos2, BULLET_VELOCITY, _rotation, DOUBLEBARREL_LIFETIME);
        
        check_bullet_wall_collision(bullet1);
        check_bullet_wall_collision(bullet2);
        
        pickup_bullets.push_back(bullet1);
        pickup_bullets.push_back(bullet2);
    }

    game_window->play_sound_effect(Assets_Dir + SLASH + Tank_Shot + Wav);
}

void Bullets_Repository::move(Bullet* b) 
{
    b->lifetime -= DELAY_PER_FRAME;
    b->position += Point(b->v_x, b->v_y);
}

void Bullets_Repository::check_bullet_wall_collision(Bullet* b) 
{
    int max_p = max(HALF_BULLET_SIZE, BULLET_VELOCITY);

    for(int i = 0; i <  map->get_walls().size(); i++) 
    {
        Wall w =  map->get_walls()[i];

        if(abs(b->position.y - w.src.y) < max_p && b->v_y <= 0 &&
           b->position.x <= w.dest.x + HALF_BULLET_SIZE &&
           b->position.x >= w.src.x - HALF_BULLET_SIZE)
        {
            b->v_y = -b->v_y;
            b->position.y = w.src.y + max_p;
        }

        else if(abs(w.src.y - b->position.y) < max_p && b->v_y >= 0 && 
                b->position.x <= w.dest.x + HALF_BULLET_SIZE &&
                b->position.x >= w.src.x - HALF_BULLET_SIZE)
        {
            b->v_y = -b->v_y;
            b->position.y = w.src.y - max_p;
        }

        if(abs(b->position.x - w.src.x) < max_p && b->v_x <= 0 &&
           b->position.y <= w.dest.y + HALF_BULLET_SIZE &&
           b->position.y >= w.src.y - HALF_BULLET_SIZE) 
        {
            b->v_x = -b->v_x;            
            b->position.x = w.src.x + max_p;
        }
        
        else if(abs(w.src.x - b->position.x) < max_p && b->v_x >= 0 && 
                b->position.y <= w.dest.y + HALF_BULLET_SIZE &&
                b->position.y >= w.src.y - HALF_BULLET_SIZE)
        {
            b->v_x = -b->v_x;
            b->position.x = w.src.x - max_p;
        }

    }
}

void Bullets_Repository::check_all_bullets()
{
    for(int i = 0; i < pickup_bullets.size(); i++) 
    {
        Bullet* b = pickup_bullets[i];

        if(b->has_shield == true || b->lifetime <= 0) 
        {
            delete b;
            pickup_bullets.erase(pickup_bullets.begin() + i);
            --i;
            continue;
        }

        move(pickup_bullets[i]);
        check_bullet_wall_collision(pickup_bullets[i]);
    }

    for(int i = 0; i < tank_bullets.size(); i++) 
    {
        for(int j = 0; j < tank_bullets[i].size(); j++) 
        {
            Bullet* b = tank_bullets[i][j];
            
            if(b->has_shield == true || b->lifetime <= 0) 
            {
                delete b;
                tank_bullets[i].erase(tank_bullets[i].begin() + j);
                --j;
                continue;
            }

            move(tank_bullets[i][j]);
            check_bullet_wall_collision(tank_bullets[i][j]);
        }
    }
}

void Bullets_Repository::check_gifs() 
{   
    if (gifts.size() != 0)
    {
        for(int i = 0; i < gifts.size(); i++) 
        {
            if(gifts[i]->lifetime <= 0 || gifts[i]->tank_collision) 
            {
                Gift* p = gifts[i];
                delete p;
                gifts.erase(gifts.begin() + i);
                --i;
            }
        }
    }
}

void Bullets_Repository::provide_gifts()
{
    if(gifts.size() == 3) 
        return;
    
    if(rand() % TIME_UNIT / 10 == 0) 
    {
        int random_type = rand() % 3;
        
        ++random_type;

        Point win = map->get_dimensions();

        Point pos(rand() % (3 * int(win.x)), rand() % (3 * int(win.y)));
            
        Gift* p = new Gift(pos, random_type);
        
        gifts.push_back(p);
    }    
}

void Bullets_Repository::draw() 
{
    draw_gifs();    
    draw_all_bullets();
}

void Bullets_Repository::draw_all_bullets()
{
    for(int i = 0; i < tank_bullets.size(); i++) 
        for(int j= 0; j < tank_bullets[i].size(); j++) 
            game_window->fill_circle(tank_bullets[i][j]->position, HALF_BULLET_SIZE, WHITE);

    for(int i = 0; i < pickup_bullets.size(); i++)
        game_window->fill_circle(pickup_bullets[i]->position, HALF_BULLET_SIZE, WHITE);
}

void Bullets_Repository::draw_gifs() 
{
    for(int i = 0; i < gifts.size(); i++) 
    {
        Gift* p = gifts[i];
        
        p->lifetime -= DELAY_PER_FRAME;
        
        game_window->draw_img(p->filename, 
            Rectangle(p->position - Point(QUARTER_TANK_SIZE, QUARTER_TANK_SIZE),
            HALF_TANK_SIZE, HALF_TANK_SIZE));
    }
}

vector <Gift*> Bullets_Repository::get_gifs() 
{ 
    return gifts; 
}

vector <Bullet*> Bullets_Repository::get_pickup_bullets()
{
    return pickup_bullets;
}

vector <vector <Bullet*>> Bullets_Repository::get_tank_bullets() 
{
    return tank_bullets;
}