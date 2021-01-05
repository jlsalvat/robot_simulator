#include "SDL.h"
#include <SDL2/SDL.h>
#include <string>
#include <stdexcept>
#include <iostream>

SDL::SDL(Uint32 flags) {
	if (SDL_Init(flags) != 0){
        std::string errorMessage = std::string("SDL_INIT: SDL could not initialize! SDL Error:")+SDL_GetError()+"\n";
        throw std::runtime_error(errorMessage);
	}
	else{
        std::cout<<"SDL_INIT OK\n";
	}
  /*      if (SDL_ShowCursor(SDL_DISABLE) < 0) {
            throw std::runtime_error("SDL_ShowCursor");
        }*/
}

SDL::~SDL() {
	SDL_Quit();
}

Uint32 SDL::WasInit(Uint32 flags) {
	return SDL_WasInit(flags);
}
