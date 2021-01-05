#pragma once
#include "../src_cpp/Timer.h"
#include "../src_cpp/Font.h"
#include "../src_cpp/Graphics.h"
#include <SDL2/SDL.h>
#include <sstream>
#include <string>
class Fps
{
public :
	Fps(const Graphics &graphics, std::string font="arial", int size=20, SDL_Color c = SDL_Color{ 200,170,200 });
	void update();
	void draw(int pos_x, int pos_y);
private:
	Graphics _graphics;
	Timer _tim_fps;
	float _avgFPS=0;
	int _countedFrames = 0;
	Font _font;
	SDL_Color _color;
	//In memory text stream
	std::stringstream _timeText;
};

