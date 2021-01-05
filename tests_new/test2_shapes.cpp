/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
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

enum class state : unsigned int { stop, start, pause };

// Initialisation
using namespace std;
int main(int argc, char* args[])
{
	Timer tim;
	int x = Parameters::SCREEN_WIDTH / 2;
	int y = Parameters::SCREEN_HEIGHT / 2;
	state state_chronometre = state::stop;
	SDL_Event event;
	Input input;
	SDL sdl;
	Graphics graphics(200, 200);
	vector <float> positions_capteurs_cm = { 0.5,6.0,8.0,12.5 };
	Color c = { 200,0,0,255 };
	Cross cross(c);
	Rectangle rect_inside1(97, 97, 10, 20);
	Rectangle rect_inside2(98, 98, 10, 20);
	Rectangle rect_inter1(99, 100, 10, 20);
	Rectangle rect_inter2(99, 90, 10, 20);
	Rectangle rect_inter3(97, 99, 20, 10);
	Rectangle rect_inter4(97, 100, 20, 10);
	Rectangle rect_inter5(99, 90, 20, 10);
	Rectangle rect_outside(10, 10, 20, 10);

	SDL_SetRenderDrawColor(graphics.gRenderer(), 100, 100, 100, 0);
	bool running = true;
	//While application is running
	int angle = 0;
	while (running)
	{
		input.beginNewFrame();
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				input.keyDownEvent(event);
				break;
			case SDL_KEYUP:
				input.keyUpEvent(event);
				break;
			case SDL_QUIT:
				running = false;
				break;
			default:
				break;
			}
		}
		if (input.wasKeyPressed(SDLK_q))
		{
			running = false;
		}
		if (input.wasKeyPressed(SDLK_ESCAPE))
		{
			running = false;
		}
		//Clear screen

		if (input.wasKeyPressed(SDLK_SPACE))
		{
			//dragon_marche.draw(graphics,x,y,0,nullptr,SDL_FLIP_HORIZONTAL);
			if (state_chronometre == state::stop) {
				state_chronometre = state::start;
				tim.start();
			}
			else
				if (state_chronometre == state::start) {
					state_chronometre = state::pause;
					tim.pause();
				}
				else
					if (state_chronometre == state::pause) {
						state_chronometre = state::start;
						tim.unpause();
					}
		}
		if (input.wasKeyPressed(SDLK_r))
		{
			//dragon_marche.draw(graphics,x,y,0,nullptr,SDL_FLIP_HORIZONTAL);
			tim.reset();
			tim.stop();
			state_chronometre = state::stop;
		}

		SDL_SetRenderDrawColor(graphics.gRenderer(), 120, 120, 120, 255);
		graphics.clear();
		cross.update(100, 100);
		float value;
		bool test=cross.isInside(rect_inside1);
			value = cross.collidesWith(rect_inside1);
		test = cross.isInside(rect_inside2);
  	value = cross.collidesWith(rect_inside2);
		test = cross.isInside(rect_inter1);
		if (test == false)
			value = cross.collidesWith(rect_inter1);
		test = cross.isInside(rect_inter2);
		if (test == false)
			value = cross.collidesWith(rect_inter2);
		test = cross.isInside(rect_inter3);
		if (test == false)
			value = cross.collidesWith(rect_inter3);
		test = cross.isInside(rect_outside);
		value = cross.collidesWith(rect_inter4);
		value = cross.collidesWith(rect_inter5);
		if (test == false)
			value = cross.collidesWith(rect_outside);
		Polyline poly1(100, 100);
		poly1.add_point(102, 98);
		poly1.add_point(104, 96);
		poly1.add_point(124, 96);
		poly1.add_point(122, 98);
		poly1.add_point(120, 100);
		cross.update(80, 80);
		value = cross.collidesWith(poly1);
		cross.update(98, 100);
		value = cross.collidesWith(poly1);
		cross.update(102, 98);
		value = cross.collidesWith(poly1);
		cross.update(102,100);
		value = cross.collidesWith(poly1);
		cross.update(118,98);
		value = cross.collidesWith(poly1);

//		Rectangle rect = cross.getRectangle({ 0,0,255,255 });
//		rect.draw(graphics);
		cross.draw(graphics);
		//Update screen
		graphics.flip();
	}

	return 0;
}


