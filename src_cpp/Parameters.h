#pragma once
#include <string>
#include <SDL2/SDL.h>
#include "Utils.h"


class Parameters {
    public:
		static float convWidthPixToCm(int width) { return float(width) / RATIO_PIX_TO_CM; }
		static float convHeightPixToCm(int height)  { return ( SCREEN_HEIGHT - height) / RATIO_PIX_TO_CM; }
		static int convWidthCmToPix(float width)  {return width * RATIO_PIX_TO_CM;}
		static int convHeightCmToPix(float height)  { return  SCREEN_HEIGHT - height * RATIO_PIX_TO_CM; }
		static Point conv_SDL_Point_to_Point(SDL_Point p) {
			Point point;
			point.x_cm = Parameters::convWidthPixToCm(p.x);
			point.y_cm = Parameters::convHeightPixToCm(p.y);
			return point;
		}
		static SDL_Point conv_point_to_SDL_Point( Point p) {
			SDL_Point point;
			point.x = Parameters::convWidthCmToPix(p.x_cm);
			point.y= Parameters::convHeightCmToPix(p.y_cm);
			return point;
		}
         static const int  SCREEN_WIDTH=1280;
        static const int SCREEN_HEIGHT=768;
		static const std::string FONT_FILE;

		static constexpr  float DRAGON_SPEED = 0.5;//speed in cm
        static const std::string SCREEN_NAME;
		static constexpr  float FPS = 60; //frame per second

		static constexpr SDL_Color COLOR_BLACK = { 0, 0, 0, 0 };
		static constexpr SDL_Color COLOR_RED = { 255, 0, 0, 0 };
		static constexpr SDL_Color COLOR_GREEN = { 0, 255, 0, 0 };
		static constexpr SDL_Color COLOR_BLUE = { 0, 0, 255, 0 };

private :
	static constexpr  float RATIO_PIX_TO_CM = 5.0;//un cm = 5 pix
};
