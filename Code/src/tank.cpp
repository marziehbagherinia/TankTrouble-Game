#include "tank.hpp"
#include "handler.hpp"

Tank::Tank(int _id, string _status, Point _position, double _rotation,
           Map* _map, Bullets_Repository* b, Window* win)
: id(_id),
  status(_status),
  position(_position),
  rotation(_rotation),
  map(_map),
  all_bullets(b),
  game_window(win)
{
    filename = Assets_Dir + SLASH + Generic_Tank + to_string(id) + DASH + status + Extension;
}

bool Tank::check_wall_collision() 
{
    vector <Wall> walls = map->get_walls();

    for(int i = 0; i < walls.size(); i++) 
    {
        Wall w = walls[i];
        
        if((abs(position.x - w.src.x) <= HALF_TANK_SIZE) &&
          (position.y >= w.src.y - HALF_TANK_SIZE) && 
          (position.y <= (w.dest.y + HALF_TANK_SIZE)))
            return true; 

        else if((abs(position.y - w.src.y) <= HALF_TANK_SIZE) &&
               (position.x >= w.src.x - HALF_TANK_SIZE) && 
               (position.x <= (w.dest.x + HALF_TANK_SIZE)))
            return true;
    }
    return false;
}

bool Tank::check_tanks_collision(Tank& second_tank) 
{
    double distance = get_distance(position, second_tank.position);
    
    if(distance < TANK_SIZE) 
        return true;
    else 
        return false;
}

void Tank::check_bullet_collision() 
{
    vector < vector <Bullet*>> b = all_bullets->get_tank_bullets();
    
    for(int i = 0; i < b.size(); i++) 
    {
        for(int j = 0; j < b[i].size(); j++) 
        {
            const Point bp = b[i][j]->position;
            
            if(get_distance(position, bp) < HALF_TANK_SIZE) 
            {
                if(status == SHIELD) 
                    b[i][j]->has_shield = true;
                else
                    throw Collision_Ex(id);
            }
        }
    }

    vector <Bullet*> p = all_bullets->get_pickup_bullets();
    
    for(int i = 0; i < p.size(); i++) 
    {
        const Point bp = p[i]->position;
       
        if(get_distance(position, bp) < HALF_TANK_SIZE) 
        {
            if(status == SHIELD) 
                p[i]->has_shield = true;
            else 
                throw Collision_Ex(id);
        }
    }
}

void Tank::check_gif_collision()
{
    vector <Gift*> p = all_bullets->get_gifs();
    
    if(p.size() == 0) 
        return;

    for(int i = 0; i < p.size(); i++) 
    {
        if(get_distance(position, p[i]->position) < (GRID_SIZE / 4)) 
        {
            p[i]->tank_collision = true;
            all_bullets->check_gifs();
            
            string type = p[i]->type;
            
            if(type == SHIELD) 
            {
                Gift gift(type, 1, SHIELD_LIFETIME);
                taken_gifs.push_back(gift);
            }

            else if(type == DOUBLEBARREL) 
            {
                Gift gift(type, 10, 5 * TIME_UNIT);
                taken_gifs.push_back(gift);
            }

            else if(type == SHOTGUN) 
            {
                Gift gift(type, 3, 1.5 * TIME_UNIT);
                taken_gifs.push_back(gift);
            }

            break;
        }
    }
}

int Tank::get_rotation_state()
{
    if(rotate_cw) 
        return 1; 
    else if(rotate_ccw)
        return -1;
    else
        return 0;
}

void Tank::set_position(Point p)
{
    position = p;
}

void Tank::set_status()
{
    if(taken_gifs.size() == 0) 
    {
        status = NORMAL;
        return;
    }

    else 
        status = taken_gifs[0].type;
}

void Tank::rotate_clockwise(bool state)
{
    rotate_cw = state;
}

void Tank::rotate_counterclockwise(bool state)
{
    rotate_ccw = state;
}

void Tank::moving_forward(bool state)
{
    move_frw = state;
}

void Tank::moving_backward(bool state)
{
    move_backward = state;
}

void Tank::shoot()
{    
    if(status == NORMAL || status == SHIELD)
        all_bullets->add_to_bullets(id, position, rotation);

    else if(status == DOUBLEBARREL || status == SHOTGUN) 
    {
        all_bullets->add_to_pickup_bullets(position, rotation, status);
        --taken_gifs[0].shoot_count;
    }
}

void Tank::move()
{
    int v_x = (double)TANK_VELOCITY * cos(((double)rotation * PI) / PIANGLE);
    int v_y = (double)TANK_VELOCITY * sin(((double)rotation * PI) / PIANGLE);

    if(move_frw) 
        position =  position + Point(v_x, v_y);

    if(move_backward) 
        position =  position - Point(v_x, v_y);
}

void Tank::move_back()
{
    int v_x = (double)TANK_VELOCITY * cos((rotation * PI) / PIANGLE);
    int v_y = (double)TANK_VELOCITY * sin((rotation * PI) / PIANGLE);

    if(move_frw) 
        position =  position - Point(v_x, v_y);
    
    if(move_backward) 
        position =  position + Point(v_x, v_y);   
}

void Tank::draw(Tank& second_tank)
{
    int direction = get_rotation_state();
    rotation += direction * ANGULAR_VELOCITY;

    set_status();
    filename = Assets_Dir + SLASH + Generic_Tank + to_string(id) + DASH + status + Extension;
    move();
    
    if(check_wall_collision()) 
        move_back();

    else if(check_tanks_collision(second_tank)) 
        move_back();
    
    game_window->draw_img(filename, 
       Rectangle(position - Point(HALF_TANK_SIZE, HALF_TANK_SIZE),TANK_SIZE, TANK_SIZE), 
       NULL_RECT, rotation);
}

void Tank::update_taken_gifs()
{
    if(taken_gifs.size() == 0) 
        return;
    
    string type = taken_gifs[0].type;
    
    if(type == SHIELD) 
    {
        if(taken_gifs[0].lifetime < 0) 
            taken_gifs.erase(taken_gifs.begin());
        else 
            taken_gifs[0].lifetime -= (TIME_UNIT/20);
    }

    else if(type == DOUBLEBARREL || type == SHOTGUN) 
    {
        if(taken_gifs[0].shoot_count == 0) 
        {
            taken_gifs.erase(taken_gifs.begin());
        }
    }
}
