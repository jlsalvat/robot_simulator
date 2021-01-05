#pragma once
#include <SDL2/SDL.h>
#include "Graphics.h"
#include "Shape.h"

class LTexture{
public:
    // ouverture de l'image entière (clip=null) ou non SDL_Rect clip={x,y,width,height}
	LTexture( SDL_Texture * texture = nullptr,  const SDL_Rect *clip= nullptr) ;

//constructeur par recopie interdit
	LTexture(const LTexture&)=delete;
	LTexture& operator=(const LTexture&)=delete;
//	LTexture(LTexture&& other);
//	LTexture& operator=(LTexture&& other);
~LTexture() ;
void free();
virtual void update();
//affichage selon les coordonnées en pixel à partir du haut à gauche
void draw(Graphics &graphics,int x, int y, int angle_deg=0, SDL_Point* center=nullptr, SDL_RendererFlip flip=SDL_FLIP_NONE );
//affichage selon les coordonnées en cm à partir du bas à gauche
void draw(Graphics &graphics,float x, float y,int angle_deg=0, SDL_Point* center=nullptr, SDL_RendererFlip flip=SDL_FLIP_NONE );

//change attribute of sprite
void setColor(Uint8 red, Uint8 green, Uint8 blue);
void setAlpha(Uint8 alpha);
void setVisible(bool isVisible);
void reset();

float getX() const {return mX;}
float getY() const {return mY;}
SDL_Point getDimension() const;
SDL_Texture * getTexture() const{ return mTexture; }
SDL_Rect  getClip() const{ return *mClip; }

protected:
float mX;
float mY; //position en X et Y en cm à partir du bas et gauche
SDL_Rect *mClip;
SDL_Texture *mTexture;
bool mVisible=true;
//
};
