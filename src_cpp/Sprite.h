#pragma once
#include "Graphics.h"
#include "LTexture.h"


class Sprite : public LTexture{
public:
	Sprite(SDL_Texture* texture, const SDL_Rect *clip=nullptr);
    // ouverture de l'image entière (clip=null) ou non SDL_Rect clip={x,y,width,height}
Sprite(Graphics &graphics,const std::string file_image,  const SDL_Rect *clip= nullptr) ;
};
