#include "Common.h"

void Common::verif_clip(Graphics &graphics, const SDL_Rect *clip) {

	int width = graphics.getWidth();
	int height = graphics.getHeight();
	if (clip->x<0 || clip->x>width) {
		const auto warn_str = "Sprite::Sprite : clip.x OUT OF BOUND \n";
		throw std::runtime_error(warn_str);
	}
	if (clip->y<0 || clip->y>height) {
		const auto warn_str = "Sprite::Sprite : clip.y OUT OF BOUND \n";
		throw std::runtime_error(warn_str);
	}
	if (clip->w > width) {
		const auto warn_str = "Sprite::Sprite : clip.w OUT OF BOUND \n";
		throw std::runtime_error(warn_str);
	}
	if (clip->h > height) {
		const auto warn_str = "Sprite::Sprite : clip.h OUT OF BOUND \n";
		throw std::runtime_error(warn_str);
	}
}

