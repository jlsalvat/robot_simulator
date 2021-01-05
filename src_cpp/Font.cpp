#include "Font.h"
#include <SDL2/SDL.h>
#include <cassert>

using namespace std;
/*Font::Font() {
	Font("arial");
}*/
Font::Font(TTF_Font* font) :ttf_font(font) {
//If the argument expression of this macro with functional form compares equal to zero (i.e., the expression is false), 
// a message is written to the standard error device and abort is called, terminating the program execution.
	assert(font);
}
Font::Font(const std::string& file, int ptsize, long index) {
	string file_path = "";
	file_path = "../fonts/" + Parameters::FONT_FILE + string(".ttf");
	if ((ttf_font = TTF_OpenFontIndex(file_path.c_str(), ptsize, index)) == nullptr) {
		const auto warn_str = "TTF_OpenFontIndex : Cannot load Font '" + file_path + "'!";
		throw std::runtime_error(warn_str);
	}
}

Font::~Font() {
	if (ttf_font != nullptr)
		TTF_CloseFont(ttf_font);
}

Font::Font(Font&& other) noexcept : ttf_font(other.ttf_font) {
	other.ttf_font = nullptr;
}

Font& Font::operator=(Font&& other) noexcept {
	if (&other == this)
		return *this;
	if (ttf_font != nullptr)
		TTF_CloseFont(ttf_font);
	ttf_font = other.ttf_font;
	other.ttf_font = nullptr;
	return *this;
}

TTF_Font* Font::get() const {
	return ttf_font;
}

int Font::getStyle() const {
	return TTF_GetFontStyle(ttf_font);
}

Font& Font::setStyle(int style) {
	TTF_SetFontStyle(ttf_font, style);
	return *this;
}

int Font::getOutline() const {
	return TTF_GetFontOutline(ttf_font);
}

Font& Font::setOutline(int outline) {
	TTF_SetFontOutline(ttf_font, outline);
	return *this;
}

int Font::getHinting() const {
	return TTF_GetFontHinting(ttf_font);
}

Font& Font::setHinting(int hinting) {
	TTF_SetFontHinting(ttf_font, hinting);
	return *this;
}

SDL_Point Font::GetSizeText(const std::string& text) const {
	int w, h;
	SDL_Point point;
	if (TTF_SizeText(ttf_font, text.c_str(), &w, &h) != 0)
		throw std::runtime_error("TTF_SizeText");
	point.x = w;
	point.y = h;
	return point;
}
SDL_Surface* Font::loadSurface(const std::string& Font, SDL_Color fg) {
	SDL_Surface* surface;
	surface = TTF_RenderText_Solid(ttf_font, Font.c_str(), fg);
	if (surface == nullptr)
		throw std::runtime_error("TTF_RenderFont_Solid");
	return surface;
}

SDL_Surface* Font::loadSurface(const std::string& Font, SDL_Color fg, SDL_Color bg) {
	SDL_Surface* surface;
	surface = TTF_RenderText_Shaded(ttf_font, Font.c_str(), fg, bg);
	if (surface == nullptr)
		throw std::runtime_error("TTF_RenderFont_Shaded");
	return surface;
}






