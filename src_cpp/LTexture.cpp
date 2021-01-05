#include "LTexture.h"

//bounding_box est le rectangle de la texture, pour l'instant on n'a pas sa position en x, y.
LTexture::LTexture( SDL_Texture* texture, const SDL_Rect *clip) :mTexture(texture)
{
	if (clip != NULL) {
		mClip = new SDL_Rect;
		mClip->x = clip->x;
		mClip->y = clip->y;
		mClip->w = clip->w;
		mClip->h = clip->h;
	}

}

LTexture::~LTexture(){
	free();
}

void LTexture::free() {
	
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
	}
}

void LTexture::update(){}

//affichage selon les coordonnées en pixel à partir du haut à gauche
void LTexture::draw(Graphics &graphics,int x, int y,int angle_deg, SDL_Point* center, SDL_RendererFlip flip ){
    mX= Parameters::convWidthPixToCm(x);//1cm = 5pix
    mY= Parameters::convHeightPixToCm(y);//1cm = 5pix
	if(mVisible)
    graphics.renderTexture(mTexture,x,y,mClip,angle_deg, center,flip);
}
//affichage selon les coordonnées en cm à partir du bas à gauche
void LTexture::draw(Graphics &graphics,float x, float y,int angle_deg, SDL_Point* center, SDL_RendererFlip flip){
    int x_tmp,y_tmp;
    mX=x;
    mY=y;
    x_tmp=Parameters::convWidthCmToPix(x);
	y_tmp = Parameters::convHeightCmToPix(y);
	if(mVisible)
    graphics.renderTexture(mTexture,x_tmp,y_tmp,mClip,angle_deg, center,flip);
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}


void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::setVisible(bool isVisible) {
	mVisible = isVisible;
}

void LTexture::reset() {
	setColor(255, 255, 255);
	setAlpha(255);
}

SDL_Point LTexture::getDimension() const {
// maladroit -> on a l'info dans _bouding_box (width et height)
		SDL_Point point;
		SDL_QueryTexture(mTexture, NULL, NULL, &point.x, &point.y);
		return point;
}




