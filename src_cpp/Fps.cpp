#include "Fps.h"
#include "Text.h"
#include <string>

using namespace std;

Fps::Fps(const Graphics &graphics,  string font, int size, SDL_Color c) : _graphics(graphics), _font(font,size),_color(c) {
	int _countedFrames = 0;
	_tim_fps.start();
}
void Fps::update() {
	_avgFPS = _countedFrames / (_tim_fps.getTicks() / 1000.f);
	if (_avgFPS > 2000000)
	{
		_avgFPS = 0;
	}
	_countedFrames++;
}
void Fps::draw(int pos_x, int pos_y) {
	_timeText.str("");
	_timeText << "Average Frames Per Second " << _avgFPS;
	Text text_time(_graphics, _timeText.str().c_str(), _font, _color);
	text_time.draw(_graphics, pos_x, pos_y);
}
