#pragma once
#include "../src_cpp/SpriteAnimated.h"
#include "../src_cpp/Shape.h"
#include "../src_cpp/TileMap.h"
#include <vector>

class AnimatedCoins : public SpriteAnimated{
public :
	AnimatedCoins(Graphics & graphics, const SDL_Rect *rect_src_total, std::vector<Circle*> vector_circle);
	void  draw(Graphics & graphics);
	int getNbCoins() { return _vector_points.size(); }
	bool collideWith(const Rectangle &other);
private : 
	std::vector<SDL_Point *> _vector_points;
	std::vector<bool> _vector_visible;

};
