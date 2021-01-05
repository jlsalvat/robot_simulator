#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include "../src_cpp/Graphics.h"
#include "../src_cpp/SDL.h"
#include "../src_cpp/Parameters.h"
#include "../src_cpp/LTexture.h"
#include "../src_cpp/Input.h"
#include "../src_cpp/Sprite.h"
#include "../src_cpp/SpriteAnimated.h"
#include "../src_cpp/Timer.h"
#include "../src_cpp/Text.h"
#include "../src_cpp/Font.h"
#include "../src_cpp/MapRobot.h"
#include "../src_cpp/MapShapes.h"
#include "../src_cpp/AnimatedCoins.h"
#include "../src_cpp/Robot.h"
#include <iostream>
#include <chrono> 
#include <ctime> 
#include <sstream>


struct Game {
	Game(std::string map,float positions_capteurs_cm[],int nb_capteurs);
	bool eventHandler();
	void  setPWM(int pwm_l, int pwm_r);
	void  setSens(int sens_l, int sens_r);
	float * getCapteurs();
	bool getJack();
	bool getFDC();
	Graphics getGraphics() const { return _graphics; };
	SDL_Point getScreenSize() { return _graphics.getSize(); }
	void update();
	void draw(bool all);
	bool isKeyPressed(SDL_Keycode key);
private:
	void course_update();
	void calcul_gain(bool all_coins);
	void drawMessage();
	SDL _sdl;
	Graphics _graphics;
	MapShapes _map_shapes;
	MapRobot _map;
	std::vector <float> _positions_capteurs_cm;
	Robot _robot;
	AnimatedCoins _coins;
	Font _font;
	Font _miniFont;
	SDL_Point _size;
	Rectangle _barre;
	Input _input;
	//données robot
	int _pwm_l, _pwm_r;
	bool _sens_l, _sens_r;
	bool _jack;
	bool _fdc;
	std::vector <float> _capteurs;
	//données course
	int _counter_coins;
	std::string _text_state, _text_jeton, _text_score;
	Timer _tim;
};
