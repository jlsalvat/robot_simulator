#include "Sprite.h"
#include "LTexture.h"
#include "Common.h"

Sprite::Sprite(SDL_Texture* texture, const SDL_Rect *clip):LTexture(texture,clip){
}
    // ouverture de l'image entière (clip=null) ou non SDL_Rect clip={x,y,width,height}
Sprite::Sprite(Graphics &graphics,const std::string file_image, const SDL_Rect *clip):LTexture(graphics.loadImage(file_image) ,clip){
        if (clip!=NULL){
            mClip = new SDL_Rect;
            mClip->x=clip->x;
            mClip->y=clip->y;
            mClip->w=clip->w;
            mClip->h=clip->h;
			Common::verif_clip(graphics, clip);
        }
}
