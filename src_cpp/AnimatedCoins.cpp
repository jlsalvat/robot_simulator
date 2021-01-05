#include "AnimatedCoins.h"

AnimatedCoins::AnimatedCoins(Graphics & graphics, const SDL_Rect *rect_src_total, std::vector<Circle*> vector_circle):
	SpriteAnimated(graphics, "coin_gold.png", rect_src_total, 8, 8) {
	for (int i = 0; i < vector_circle.size(); i++) {
		int x = vector_circle[i]->x;
		int y= vector_circle[i]->y;
		_vector_points.push_back(new SDL_Point{x,y });
		_vector_visible.push_back(true);
	}
	start();
}
void AnimatedCoins::draw(Graphics & graphics) {
	for (int i = 0; i < _vector_points.size(); i++) {
		if (_vector_visible[i] == true) 
			SpriteAnimated::draw(graphics, _vector_points[i]->x, _vector_points[i]->y);
	}
}
bool AnimatedCoins::collideWith(const Rectangle &other) {
	for (int i = 0; i < _vector_points.size(); i++) {
		if (_vector_visible[i] == true) {
			int x = _vector_points[i]->x;
			int y = _vector_points[i]->y;
			if (x <= other.getRight() && x >= other.getLeft() && y <= other.getBottom() && y >= other.getTop()) {
				_vector_visible[i] = false;
				return true;
			}
		}
	}
	return false;
}
