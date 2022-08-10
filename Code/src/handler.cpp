#include "handler.hpp"


Game::Game(string map_file_address)
{
    int win_width, win_height;
 
    map = new Map();

    prepare_map(map, map_file_address, win_width, win_height, first_tank_pos, second_tank_pos);

    game_window = new Window(win_width * GRID_SIZE + 1, win_height * GRID_SIZE + 1, TITLE);

    map->set_window(game_window);    
    
    start();
}

void Game::start()
{
    bullet_rep = new Bullets_Repository(2, map, game_window);

    first_tank = Tank(1, NORMAL, first_tank_pos, rand() % 360, map, bullet_rep, game_window);
    second_tank = Tank(2, NORMAL, second_tank_pos, rand() % 360, map, bullet_rep, game_window);
    
    game_window->clear();

    first_tank.draw(second_tank);
    second_tank.draw(first_tank);
    game_window->update_screen();

    handle_game();
}

void Game::handle_game()
{
    while(true) 
    {
        try 
        {
            
            update_game_window(*game_window, first_tank, second_tank);
            
            game_window->clear();
            
            map->draw();
            
            bullet_rep->check_all_bullets();
            bullet_rep->provide_gifts();
            bullet_rep->draw();

            first_tank.draw(second_tank);
            second_tank.draw(first_tank);

            check_tank_situation(first_tank);
            check_tank_situation(second_tank);

            game_window->update_screen();

            delay(DELAY_PER_FRAME);

        } catch(Quit_Game_Ex& qg) { 
            return; 
        } catch(Collision_Ex& fc) {
            
            RGB color(0, 0, 0);
            
            if(fc.tank_id == 1) 
                color = GREEN;
            else if(fc.tank_id == 2) 
                color = RED;

            string text = TANK + to_string(fc.tank_id) + LOST;
            
            Point text_place(game_window->get_width() / 4, game_window->get_height() / 2);
            
            game_window->play_sound_effect(Assets_Dir + SLASH + Tank_Explosion + Wav);
            game_window->show_text(text, text_place, color, "Assets/TankTrouble.ttf", 48); 
            game_window->update_screen();
            
            delay(1500);
            return;
        }
    }
}

double get_distance(const Point& p1,const Point& p2) {
    double square_of_distance = 0;
    square_of_distance += pow((p1.x - p2.x), 2);
    square_of_distance += pow(p1.y - p2.y, 2);
    return sqrt(square_of_distance);
}

vector <string> provide_input(string line)
{
    string field;
    vector <string> gettingLine;

    stringstream ss(line);

    while(getline(ss,field,' '))
        gettingLine.push_back(field);

    return gettingLine;
}

void prepare_map(Map* map, string map_file_address, int& win_width, int& win_height,
                 Point& first_tank_pos, Point& second_tank_pos)
{
    ifstream map_file(map_file_address);

    string line, field;  
    int first_tank_x, first_tank_y, second_tank_x, second_tank_y;
    vector <string> gettingLine, wall_alignment;

    getline(map_file, line);
    win_height = stoi(provide_input(line)[0]);
    win_width = stoi(provide_input(line)[1]);
    map->set_dimensions(win_width, win_height);

    gettingLine.clear();

    for(int i = 0; i < win_height; i++) 
    {
        string map_line;
        getline(map_file, map_line);
        wall_alignment.push_back(map_line);
    }

    getline(map_file, line);
    first_tank_x = stoi(provide_input(line)[0]);
    first_tank_y = stoi(provide_input(line)[1]);

    gettingLine.clear();

    getline(map_file, line);
    second_tank_x = stoi(provide_input(line)[0]);
    second_tank_y = stoi(provide_input(line)[1]);

    first_tank_pos = Point((first_tank_y + 0.5) * GRID_SIZE, (first_tank_x + 0.5) * GRID_SIZE);
    second_tank_pos = Point((second_tank_y + 0.5) * GRID_SIZE, (second_tank_x + 0.5) * GRID_SIZE);

    make_map(map, wall_alignment);
}

void make_map(Map* map, const vector <string>& wall_alignment)
{
    for(int i = 0; i < wall_alignment.size(); i++) 
    {
        for(int j = 0; j < wall_alignment[i].size(); j++) 
        {
            switch(wall_alignment[i][j]) 
            {
                case ZERO : 
                    break;

                case ONE : 
                {
                    Wall upper_wall( Point(j * GRID_SIZE, i * GRID_SIZE), Point((j + 1) * GRID_SIZE, i * GRID_SIZE) );
                    map->add_to_walls(upper_wall);
                    break;
                }

                case TWO: 
                {
                    Wall left_wall( Point(j * GRID_SIZE, i * GRID_SIZE), Point(j * GRID_SIZE, (i + 1) * GRID_SIZE) );
                    map->add_to_walls(left_wall);
                    break;
                }

                case THREE :
                {
                    Wall upper_wall( Point(j * GRID_SIZE, i * GRID_SIZE), Point((j + 1) * GRID_SIZE, i * GRID_SIZE) );
                    Wall left_wall( Point(j * GRID_SIZE, i * GRID_SIZE), Point(j * GRID_SIZE, (i + 1) * GRID_SIZE) );
                    map->add_to_walls(upper_wall);
                    map->add_to_walls(left_wall);
                    break;
                }
            }
        }
    }

    complete_map(map, wall_alignment);

}

void complete_map(Map* map, const vector <string>& wall_alignment)
{
    int map_height = wall_alignment.size();
    int map_width = wall_alignment[0].size();

    Point down_left(0, map_height * GRID_SIZE);   
    Point down_right(map_width * GRID_SIZE, map_height * GRID_SIZE);
    Point top_right(map_width * GRID_SIZE, 0);
  
    Wall bottom_wall(down_left, down_right);    
    Wall right_wall(top_right, down_right);

    map->add_to_walls(bottom_wall);
    map->add_to_walls(right_wall);
    map->walls_integration();
}

void check_tank_situation(Tank& tank)
{
    tank.check_bullet_collision();
    tank.check_gif_collision();
    tank.update_taken_gifs();
}

void set_next_move(Event& event, Tank& first_tank, Tank& second_tank, bool is_press)
{
    if(event.get_pressed_key() == TANK1CCW)   first_tank.rotate_counterclockwise(is_press);
    if(event.get_pressed_key() == TANK1CW)    first_tank.rotate_clockwise(is_press);
    if(event.get_pressed_key() == TANK1FRW)   first_tank.moving_forward(is_press);
    if(event.get_pressed_key() == TANK1BACK)  first_tank.moving_backward(is_press);
    

    if(event.get_pressed_key() == TANK2CCW)   second_tank.rotate_counterclockwise(is_press);
    if(event.get_pressed_key() == TANK2CW)    second_tank.rotate_clockwise(is_press);
    if(event.get_pressed_key() == TANK2FRW)   second_tank.moving_forward(is_press);
    if(event.get_pressed_key() == TANK2BACK)  second_tank.moving_backward(is_press);         
    
    if (is_press == true)
    {
        if(event.get_pressed_key() == TANK1SHOOT) first_tank.shoot();
        if(event.get_pressed_key() == TANK2SHOOT) second_tank.shoot();
    }
}

void update_game_window(Window& game_window, Tank& first_tank, Tank& second_tank) 
{
    while(game_window.has_pending_event()) 
    {
        Event event = game_window.poll_for_event();

        switch(event.get_type()) 
        {
            case Event::QUIT : 
                throw Quit_Game_Ex();
            
            case Event::KEY_RELEASE :
            {
                set_next_move(event, first_tank, second_tank, false);

                break;
            }

            case Event::KEY_PRESS : 
            {
                set_next_move(event, first_tank, second_tank, true);            
                
                break;
            }
        }    
    }
}