#ifndef __BULLET_HPP__
#define __BULLET_HPP__

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "rsdl.hpp"
#include "map.hpp"

using namespace std;

#define TANK_SIZE 80
#define TIME_UNIT 1000
#define GRID_SIZE 240
#define HALF_TANK_SIZE 40
#define QUARTER_TANK_SIZE 20
#define PICKUP_SIZE 30
#define BULLET_SIZE 8
#define HALF_BULLET_SIZE 4

#define BULLET_LIFETIME 10000
#define PICKUP_LIFETIME 5000 
#define DOUBLEBARREL_LIFETIME 5000
#define SHOTGUN_LIFETIME 1500
#define SHIELD_LIFETIME 6000

#define ANGULAR_VELOCITY 2
#define TANK_VELOCITY 3
#define BULLET_VELOCITY 4
#define PICKUP_BULLET_VELOCITY 6

#define DELAY_PER_FRAME 33

#define PI 3.14159265
#define PIANGLE 180.00
#define RIGHTANGLE 90

#define NORMAL "Normal"
#define SHIELD "Shield"
#define DOUBLEBARREL "DoubleBarrel"
#define SHOTGUN "Shotgun"
#define LASER "Laser"

#define TRUE 0
#define FALSE 1

#define SLASH "/"
#define DASH "-"

const string Assets_Dir = "Assets";
const string Generic_Tank = "Tank";
const string Generic_Pickup = "Pickup";
const string Tank_Shot = "TankShot";
const string Tank_Explosion = "TankExplosion";
const string Extension = ".png";
const string Wav = ".wav";

const vector<string> Tank_Types {"Normal", "Shield", "DoubleBarrel", "Shotgun", "Laser"}; 

class Gift
{

    public:
        
        Gift(Point _position, int t);
        Gift(string _type, int _count, int _time);    

        Point position;

        int lifetime = PICKUP_LIFETIME;
        int shoot_count;        
        bool tank_collision;
        
        string type;
        string filename;
};

class Bullet
{
    public:

        Bullet(Point _position, int _v, double _rotation, int _lifetime);

        Point position;
        
        int lifetime;
        int v_x, v_y;
        
        bool has_shield;
    
};


class Bullets_Repository
{
    public:

        Bullets_Repository(int tank_count, Map* _map, Window* _win);
        
        void add_to_bullets(int tank_id, Point _position, double _rotation);
        void add_to_pickup_bullets(Point _position, double _rotation, string gif);
        void provide_gifts();
        void move(Bullet* b);
        void draw();
        void draw_gifs();        
        void draw_all_bullets();        
        void check_gifs();
        void check_all_bullets();
        void check_bullet_wall_collision(Bullet* b);

        vector <Gift*> get_gifs();        
        vector <Bullet*> get_pickup_bullets();
        vector <vector <Bullet*>> get_tank_bullets();

    private:
        Map* map;
        Window* game_window;

        vector <Gift*> gifts;           
        vector <Bullet*> pickup_bullets;
        vector <vector <Bullet*>> tank_bullets;
};

#endif