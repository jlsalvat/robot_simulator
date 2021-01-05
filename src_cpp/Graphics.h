
#pragma once

#include <SDL2/SDL.h>
#include <map>
#include <string>
#include "Font.h"
#include "Parameters.h"

class Graphics
{
    public:
    Graphics(int width=Parameters::SCREEN_WIDTH, int height=Parameters::SCREEN_HEIGHT);
    ~Graphics();

    Graphics(const Graphics&)=default;
    Graphics& operator=(const Graphics&)=default;

    void renderTexture(SDL_Texture *tex,
            const SDL_Rect dst,
            const SDL_Rect *clip=nullptr,
            const int angle_deg=0, const SDL_Point* center=nullptr, const SDL_RendererFlip flip=SDL_FLIP_NONE) const;
    void renderTexture(SDL_Texture *tex,
            const int x, const int y,
            const SDL_Rect *clip=nullptr,
            const int angle_deg=0, const SDL_Point* center=nullptr, const SDL_RendererFlip flip=SDL_FLIP_NONE ) const;

    SDL_Texture* loadImage(const std::string& file_name,
            const bool black_is_transparent=false);
	SDL_Texture*  loadText(const std::string &text, const Font &font, SDL_Color fg = SDL_Color{ 0,0,0,0 });
	SDL_Texture*  loadText(const std::string &text,   const Font &font, SDL_Color fg, SDL_Color bg);
    void flip() const;
    void clear() const;
    SDL_Renderer* gRenderer() const  { return sdlRenderer; }
    int getWidth() const { return width_;}
    int getHeight() const { return height_;}
	SDL_Point getSize() { 
		SDL_Point size; 
		size.x = width_; 
		size.y = height_; 
		return size; 
	}
	void resize(int width, int height) {
		 SDL_SetWindowSize(sdlWindow, width, height);
		width_ = width;
		height_ = height;
	}
private:
    std::map<std::string , SDL_Texture* > sprites_sheets_;
    SDL_Window *sdlWindow;
    SDL_Renderer *sdlRenderer;
    int width_;
    int height_;
};


