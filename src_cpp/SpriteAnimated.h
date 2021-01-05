#pragma once

#include "Sprite.h"
#include "Timer.h"
#include <cstdint>


struct SpriteAnimated : public Sprite {
    SpriteAnimated(
        Graphics& graphics,
        const std::string& file_name,
        const SDL_Rect* rect_source,
         int fps,  int num_frames
            );

    ~SpriteAnimated()=default;

    void start();

    void stop();

    void update() override;

	void blink(int freq);

private:
    Timer mTimer;
    int mNum_frames;
    int mCurrent_frame;
    uint32_t mTime_between_frame;
	int mFreq=0;
	int cptFreq = 0;
	bool tmpVisible = true;
};

