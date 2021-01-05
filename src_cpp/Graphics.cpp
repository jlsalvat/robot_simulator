#include <stdexcept>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "graphics.h"
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;

Graphics::Graphics(int width , int height )
{
    width_=(width<0)?Parameters::SCREEN_WIDTH:width;
    height_=(height<0)?Parameters::SCREEN_HEIGHT:height;
    sdlWindow =SDL_CreateWindow(
                   Parameters::SCREEN_NAME.c_str(),
                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                   width_,
                   height_,
                   SDL_WINDOW_RESIZABLE
               );
    if (sdlWindow == nullptr)
    {
        throw std::runtime_error("SDL_CreateWindow");
    }else
    std::cout<<"SDL_CreateWindow OK \n";
    sdlRenderer =SDL_CreateRenderer(
                     sdlWindow,
                     -1, SDL_RENDERER_ACCELERATED| SDL_RENDERER_PRESENTVSYNC);


    if (sdlRenderer == nullptr)
    {
        throw std::runtime_error("SDL_CreateRenderer");
    }else    std::cout<<"SDL_CreateRenderer OK \n";
	if (TTF_Init() == -1)
	{
		std::string text("SDL_ttf could not initialize! SDL_ttf Error: ");
		text += TTF_GetError() + std::string("\n");
		throw std::runtime_error(text.c_str());

	}
}

Graphics::~Graphics()
{
    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
}

SDL_Texture* Graphics::loadImage(const std::string& file_name, bool black_is_transparent)
{
    string delimiter = ".";
    int iDelimiter=file_name.find(delimiter);
    int iEnd= file_name.length();
    string file_path="";
    string extension = file_name.substr(iDelimiter+1,iEnd);
    if(extension =="pbm"){
        cout<<"pbm";
        file_path="../images/cave/" + file_name ;
    }else
        file_path="../images/" + file_name ;
    // spritesheet not loaded
    if (sprites_sheets_.count(file_path) == 0) {
        SDL_Texture *t=NULL;
        if (black_is_transparent) { //specificité des images cave story
            SDL_Surface *surface = SDL_LoadBMP(file_path.c_str());
            if (surface == nullptr) {
                const auto warn_str = "Cannot load texture '" + file_path + "'!";
                throw std::runtime_error(warn_str);
            }
            const Uint32 black_color = SDL_MapRGB(surface->format, 0, 0, 0);
            SDL_SetColorKey(surface, SDL_TRUE, black_color);
            t = SDL_CreateTextureFromSurface(sdlRenderer, surface);
            SDL_FreeSurface(surface);
        } else {
			//Load image at specified path
			SDL_Surface* loadedSurface = IMG_Load(file_path.c_str());
			if (loadedSurface == NULL)
			{
				const auto warn_str = "Unable to load image " + file_path + string("SDL_image Error :")+ IMG_GetError();
				throw std::runtime_error(warn_str);
			}
			else
			{
				//Color key image
				SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

				//Create texture from surface pixels
				t = SDL_CreateTextureFromSurface(sdlRenderer, loadedSurface);
				//Get rid of old loaded surface
			  //  SDL_FreeSurface( loadedSurface );
			}
        }
        if (t == nullptr) {
            throw std::runtime_error("Cannot load texture!");
        }
        sprites_sheets_[file_name] = t;
    }
    return sprites_sheets_[file_name];
}

SDL_Texture*  Graphics::loadText(const std::string &text, const Font &font, SDL_Color fg){
		//Render text surface
	SDL_Surface* surface_text = const_cast <Font&>(font).loadSurface(text, fg);
	SDL_Texture* texture_text = SDL_CreateTextureFromSurface(sdlRenderer, surface_text);
	if (texture_text == nullptr)
		throw std::runtime_error("SDL_CreateTextureFromSurface in Graphics::loadText");
	//Get rid of old surface
	//SDL_FreeSurface( textSurface );		
	return texture_text;
}
SDL_Texture*  Graphics::loadText(const std::string &text, const  Font &font, SDL_Color fg, SDL_Color bg) {
	//Render text surface
	SDL_Surface* surface_text = const_cast <Font&>(font).loadSurface(text, fg,bg);
	SDL_Texture* texture_text = SDL_CreateTextureFromSurface(sdlRenderer, surface_text);
	if (texture_text == nullptr)
		throw std::runtime_error("SDL_CreateTextureFromSurface in Graphics::loadText");
	//Get rid of old surface
	//SDL_FreeSurface( textSurface );		
	return texture_text;
}

void Graphics::renderTexture(
    SDL_Texture *tex,
    const SDL_Rect dst,
    const SDL_Rect *clip,
    const int angle_deg, const SDL_Point* center, const SDL_RendererFlip flip ) const
{
    if(angle_deg!=0 || flip!=SDL_FLIP_NONE){
            SDL_RenderCopyEx( sdlRenderer, tex, clip, &dst, angle_deg, center, flip );
    }
    else
      SDL_RenderCopy(sdlRenderer, tex, clip, &dst);
}

/**
 * Draw an SDL_Texture at some destination rect taking a clip of the
 * texture if desired
 */
void Graphics::renderTexture(
    SDL_Texture *tex,
    const int x,
    const int y,
    const SDL_Rect *clip,
    const int angle_deg, const SDL_Point* center, const SDL_RendererFlip flip ) const
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != nullptr)
    {
        dst.w = clip->w;
        dst.h = clip->h;
    }
    else
    {
        SDL_QueryTexture(tex, nullptr, nullptr, &dst.w, &dst.h);
    }
    this->renderTexture(tex, dst, clip,angle_deg,center,flip);
}

void Graphics::flip() const
{
    SDL_RenderPresent(sdlRenderer);
}

void Graphics::clear() const
{
    SDL_RenderClear(sdlRenderer);
}
