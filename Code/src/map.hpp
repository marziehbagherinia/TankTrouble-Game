#ifndef __MAP_HPP__
#define __MAP_HPP__
#include "rsdl.hpp"
#include <vector>
#include <string>

using namespace std;

class Wall
{
    public:
        Wall(Point _src, Point _dest) : src(_src), dest(_dest) {}
        Point src, dest;
};

class Map
{
    public:

            Map();
            Map(Window* win);
            
            Point get_dimensions();
            vector <Wall> get_walls() const;
            
            void set_dimensions(int w, int h);
            void add_to_walls(Wall w);
            void walls_integration();
            void horizon_walls_integration(vector <Wall>& horizontal);
            void vertic_walls_integration(vector <Wall>& vertical);
            void set_window(Window* win);
            void draw();

    private:

            Window* game_window;

            int width;
            int height;

            vector <Wall> walls;
};

#endif