#ifndef __TANK_HPP__
#define __TANK_HPP__

#include "bullets.hpp"
#include "exceptions.hpp"

class Tank
{
    
public:
    
    Tank() {};
    Tank(int _id, std::string status, Point _position, double _rotation, Map* _map, Bullets_Repository* b, Window* win);
    
    int get_rotation_state();

    bool check_wall_collision();
    bool check_tanks_collision(Tank& second_tank);

    void set_status();
    void set_position(Point p);
    void switch_rotation(int direction);
    void rotate_clockwise(bool state);
    void rotate_counterclockwise(bool state);
    void moving_forward(bool state);
    void moving_backward(bool state);
    void update_taken_gifs();
    void shoot();
    void move();
    void move_back();
    void draw(Tank& second_tank);
    void check_bullet_collision();
    void check_gif_collision(); 

private:

    string filename;
    string status;

    int id;
    double rotation = 0;

    bool rotate_cw = false;
    bool rotate_ccw = false;
    bool move_frw = false;
    bool move_backward = false;

    Point position;
    Map* map;
    Bullets_Repository* all_bullets;
    Window* game_window;

    vector <Gift> taken_gifs;
};

#endif