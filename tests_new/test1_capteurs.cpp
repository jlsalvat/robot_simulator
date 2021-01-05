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

enum class state : unsigned int {stop,start,pause};

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
	MapShapes map_shapes(graphics); // on récup les formes
	vector <float> positions_capteurs_cm = { 0.5,6.0,8.0,12.5 };
	MapRobot map(graphics, "../assets/robot1.tmx", map_shapes.getLignes(), map_shapes.getArcs());
	SDL_Point size = map.getSize();
	graphics.resize(size.x, size.y);
	map.printData();
	Point center;
	Direction direction;
	map.robot(center, direction);
	Robot robot(map_shapes.getImageTexture(),center,direction, positions_capteurs_cm);
	SDL_Rect rect_src_total = { 0,0,32 * 8,32 };
	AnimatedCoins coins(graphics,&rect_src_total,map.getAnimationCoins());
	vector<Rectangle> line;
	Font font("arial", 40);
	Point centre;
	centre.x_cm = center.x_cm ;
	centre.y_cm = center.y_cm -30;
	Circle cercle(centre, 30);
	cercle.color.alpha = 255;
	SDL_SetRenderDrawColor(graphics.gRenderer(), 100, 100, 100, 0);
    bool running = true;
    //While application is running
	int angle = 0;
    while( running )
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
			}else
			if (state_chronometre == state::start) {
				state_chronometre = state::pause;
				tim.pause();
			}else
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

		SDL_SetRenderDrawColor(graphics.gRenderer(),120,120,120,255);
		graphics.clear();
		robot.update(40, 20);
		
		coins.update();		
		Text text_time(graphics, tim.getTime().c_str(), font, SDL_Color{ 200,50,50 });
		text_time.draw(graphics, size.y-50, 10);
		map.draw(graphics);
		coins.draw(graphics);

		robot.draw(graphics);
		cercle.draw(graphics);
        //Update screen
        graphics.flip();
    }

    return 0;
}


