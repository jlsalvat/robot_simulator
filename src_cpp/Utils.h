#pragma once
#include <SDL2/SDL.h>
#include <cmath>
struct Point {
	float x_cm;
	float y_cm;
};

enum class Direction { NORTH, SUD, EST,WEST }; // enum class
class Utils {
public:
//getDirection between 2 points
	static Direction getDirection(SDL_Point p1, SDL_Point p2) {
		if (abs(p1.x - p2.x) <= 3) 
			return (p1.y > p2.y ? Direction::NORTH : Direction::SUD);
		else if (abs(p1.y - p2.y) <= 3) 
			return (p1.x > p2.x ? Direction::EST : Direction::WEST);
	}
	static Direction getDirection( Point p1,  Point p2) {
		if (abs(p1.x_cm - p2.x_cm) <= 1)
			return (p1.y_cm < p2.y_cm ? Direction::NORTH : Direction::SUD);
		else if (abs(p1.y_cm - p2.y_cm) <= 1)
			return (p1.x_cm > p2.x_cm ? Direction::EST : Direction::WEST);
	}

};
