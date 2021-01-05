#pragma once
#include "Graphics.h"
#include "LTexture.h"
#include "Font.h"
#include <SDL2/SDL.h>


class Text : public LTexture{
public:
	Text(SDL_Texture* texture, const SDL_Rect *clip=nullptr);
	Text(Graphics & graphics, const std::string text, const Font & font, const SDL_Color & fg, const SDL_Rect * clip = nullptr);
	Text(Graphics & graphics, const std::string text, const Font & font, const SDL_Color & fg, const SDL_Color & bg, const SDL_Rect * clip = nullptr);
	//constructeur par recopie interdit
};
