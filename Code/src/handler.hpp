#ifndef __HANDLER_HPP__
#define __HANDLER_HPP__

#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include "tank.hpp"

#define TANK1CCW   'j'
#define TANK1CW    'l'
#define TANK1FRW   'i'
#define TANK1BACK  'k'
#define TANK1SHOOT 'u'
#define TANK2CCW   'a'
#define TANK2CW    'd'
#define TANK2FRW   'w'
#define TANK2BACK  's'
#define TANK2SHOOT 'q'

#define ZERO  '0'
#define ONE   '1'
#define TWO   '2'
#define THREE '3'

#define TITLE "Tank Trouble!"
#define TANK "Tank "
#define LOST " was lost!"

void start_game(string map_file_address);
void check_tank_situation(Tank& tank);
void make_map(Map* map, const vector<string>& wall_positions);
void complete_map(Map* map, const vector <string>& wall_alignment);
void set_next_move(Event& event, Tank& first_tank, Tank& second_tank, bool is_press);
void update_game_window(Window& game_window, Tank& first_tank, Tank& second_tank);
void prepare_map(Map* map, string map_file_address, int& win_width, int& win_height, Point& first_tank_pos, Point& second_tank_pos);

double get_distance(const Point& p1, const Point& p2);

vector <string> provide_input(string line);

class Game
{
public:
	Game(string map_file_address);
	void start();
	void handle_game();

private:
	Map* map;
	Window* game_window;
	Bullets_Repository* bullet_rep;
	Tank first_tank;
	Tank second_tank;
	Point first_tank_pos, second_tank_pos;
};

#endif