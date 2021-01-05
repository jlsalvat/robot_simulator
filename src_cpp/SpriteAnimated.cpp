#include "SpriteAnimated.h"

SpriteAnimated::SpriteAnimated(
        Graphics& graphics,
        const std::string& file_name,
        const SDL_Rect* rect_source,
         int fps,  int num_frames
        ) :
    Sprite(graphics, file_name, rect_source),
    mTimer{},
    mNum_frames(num_frames),
    mCurrent_frame(0),
    mTime_between_frame(1000 / fps){
      mClip->w=mClip->w/mNum_frames;//we change de size of the sprite
    }

void SpriteAnimated::start(){
    mTimer.start();
}

void SpriteAnimated::stop(){
    mTimer.stop();
}

void SpriteAnimated::blink(int freq) {
	if (freq == 0)
		mFreq = freq;
	else
	mFreq = Parameters::FPS/freq;
}

void SpriteAnimated::update()
{
    if (mTimer.getTicks()>mTime_between_frame){
        mTimer.reset();
        ++mCurrent_frame;
        if (mCurrent_frame < mNum_frames) {
            mClip->x += mClip->w;
        } else {
            mClip->x -= mClip->w * (mNum_frames - 1);
            mCurrent_frame = 0;
        }
    }
	//si frequence>0 alors on modifie la valeur de mVisible (Sprite) en fonction de mFreq
	if (mFreq != 0) {
		cptFreq++;
		if (cptFreq >= mFreq) {
			mVisible = !mVisible;
			cptFreq = 0;
		}
	}
}
