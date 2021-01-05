#include "Text.h"
#include "Common.h"
#include "LTexture.h"

Text::Text(SDL_Texture* texture, const SDL_Rect *clip) :LTexture(texture, clip) {}
Text::Text(Graphics &graphics, const std::string text, const Font &font, const SDL_Color &fg, const SDL_Rect *clip) : LTexture(graphics.loadText(text, font, fg), clip) {
	if (clip != NULL) {
		mClip = new SDL_Rect;
		mClip->x = clip->x;
		mClip->y = clip->y;
		mClip->w = clip->w;
		mClip->h = clip->h;
		Common::verif_clip(graphics, clip);
	}
}
Text::Text(Graphics &graphics, const std::string text, const Font &font, const SDL_Color &fg, const SDL_Color &bg, const SDL_Rect *clip) :LTexture(graphics.loadText(text, font, fg,bg), clip) {

	if (clip != NULL) {
		mClip = new SDL_Rect;
		mClip->x = clip->x;
		mClip->y = clip->y;
		mClip->w = clip->w;
		mClip->h = clip->h;
		Common::verif_clip(graphics, clip);
	}
}
