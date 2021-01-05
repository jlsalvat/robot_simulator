#pragma once
#include "Graphics.h"
#include <SDL2/SDL.h>
struct Common
{
	static void  verif_clip(Graphics &graphics, const SDL_Rect *clip);
};

