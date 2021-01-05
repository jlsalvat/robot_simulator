#include "Dragon.h"
#include "Sprite.h"
#include <SDL2/SDL.h>
#include <string>
#define _USE_MATH_DEFINES
#include <cmath>
#ifndef M_PI
#define M_PI        3.141592653589793238462643383279502884L /* pi */
#endif
using namespace std;


static const  string dragon_file = "dragon.png";
static const int SPRITE_WIDTH = 64;
static const int SPRITE_HEIGHT = 70;
static const int ANIMATION_NB_FPS = 8;
static const SDL_Rect rect_src_move = { 0,0,SPRITE_WIDTH * 8,SPRITE_HEIGHT };
static const SDL_Rect rect_src_idle = { 0,SPRITE_HEIGHT,SPRITE_WIDTH * 4,SPRITE_HEIGHT };
static const SDL_Rect rect_src_jump = { 0,SPRITE_HEIGHT*2,SPRITE_WIDTH * 8,SPRITE_HEIGHT };


Dragon::Dragon(Graphics& graphics, int x, int y) :
	mIdle(graphics, dragon_file.c_str(), &rect_src_idle, ANIMATION_NB_FPS, 4),
	mMove(graphics, dragon_file.c_str(), &rect_src_move, ANIMATION_NB_FPS, 8),
	mJump(graphics, dragon_file.c_str(), &rect_src_jump, ANIMATION_NB_FPS , 8),
	mSens(Sens::right),
	mAngle(0),
	mX(Parameters::convWidthPixToCm(x)),
	mY(Parameters::convHeightPixToCm(y)),
	mBox(x,y,SPRITE_WIDTH,SPRITE_HEIGHT){
	initial_positionX = mX;
	initial_positionY = mY;
	start();
	}

void Dragon::idle() {
	mIdle.setVisible(true);//a cause du blink
	mDragon_state = waiting;
}
void Dragon::move(Sens sens, int angle) {
	mMove.setVisible(true);//a cause du blink
	mDragon_state = running;
	mSens = sens;
	mAngle = angle;
}

void Dragon::jump() {
	mJump.setVisible(true);//a cause du blink
	if (mDragon_state == running)
		mDragon_state = jumping_running;
	else
		mDragon_state = jumping_idle;
	//position initiale du saut
	x0 = mX;
	y0 = mY;
	time = 0;
	mGrounded = false;
}
// nécessaire pour avoir une intégrité des données explimées en pix et les données exprimées en cm...
void Dragon::updateBox() {
	mBox.x = Parameters::convWidthCmToPix(mX);
	mBox.y = Parameters::convHeightCmToPix(mY);
}
void Dragon::update() {

	float fTime = static_cast<float>(time) / static_cast<float>(Parameters::FPS);
	//la loi de la gravité attire le dragon vers le sol
	time++;
	mY += -GRAVITY / 5*(fTime)*(fTime);
	//s'il touche le sol, le temps utilisé pour l'accélération lors de la chute est remis à 0
	if (mGrounded) {
		time = 0;
	}
	switch (mDragon_state) {
	case waiting:
		mIdle.update();
		break;
	case jumping_running:case jumping_idle :
		mY = (-GRAVITY / 2)*(fTime)*(fTime) + V0 * (fTime) + y0;
		if(mDragon_state!=jumping_idle)
		if (mSens == Sens::right) {
			mX= V0 * (fTime) + x0;
		}
		else {
			mX = -V0 * (fTime) + x0;
		}	
		if (mGrounded) {
			mDragon_state = waiting;
		}
		mJump.update();
		break;
	case running : 
			mMove.update();
			switch (mSens) {
			case Sens::right:
				mX += Parameters::DRAGON_SPEED;
				mFlip = SDL_FLIP_NONE;
				mOffset = 0;
				break;
			case Sens::left:
				mX -= Parameters::DRAGON_SPEED;
				mFlip = SDL_FLIP_HORIZONTAL;
				mOffset = 0;
				break;
			case Sens::up:
				mY += Parameters::DRAGON_SPEED;
				mFlip = SDL_FLIP_NONE;
				mOffset = -90;
				break;
			case Sens::down:
				mY -= Parameters::DRAGON_SPEED;
				mFlip = SDL_FLIP_HORIZONTAL;
				mOffset = -90;
				break;
			}
		break;
	}
	updateBox();
	mGrounded = false;

}
void Dragon::reset() {
	blink(10*(3 - number_of_life));
	mX = initial_positionX;
	mY = initial_positionY;
	updateBox();
	setAlpha(255 - (3-number_of_life) * 60);
	mDragon_state = state::waiting;
	time = 0;
	mIdle.setVisible(true);
}

void Dragon::blink(int freq) {
	mIdle.blink(freq);
	mMove.blink(freq);
	mJump.blink(freq);
}

void Dragon::setAlpha(int alpha) {
	mIdle.setAlpha(alpha);
	mMove.setAlpha(alpha);
	mJump.setAlpha(alpha);
}

void Dragon::start() {
	mIdle.start();
	mMove.start();
	mJump.start();
}

void Dragon::draw(Graphics& graphics) {
	switch (mDragon_state) {
	case waiting:
		mIdle.draw(graphics, mX, mY, mAngle+mOffset, nullptr, mFlip);
			break;
	case running:
		mMove.draw(graphics, mX, mY, mAngle+mOffset, nullptr, mFlip);
		break;
	case jumping_running:case jumping_idle:
		mJump.draw(graphics, mX, mY, mAngle+mOffset, nullptr, mFlip);
		break;
	}
}

//void handleTileCollisions
//Handles collisions with ALL tiles the player is colliding with
void Dragon::handleTileCollisions(std::vector<Rectangle*> &others) {
	//Figure out what side the collision happened on and move the player accordingly
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = mBox.getCollisionSide(*others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->mBox.y = others.at(i)->getBottom() + 1;
				break;
			case sides::BOTTOM:
				mGrounded = true;
				this->mBox.y = others.at(i)->getTop() - this->mBox.height- 1;
				break;
			case sides::LEFT:
				this->mBox.x = others.at(i)->getRight() + 1;
				break;
			case sides::RIGHT:
				this->mBox.x = others.at(i)->getLeft() - this->mBox.width - 1;
				break;
			}
		}
		cout << "collision:" << *others.at(i) << " box=" << mBox.x << "," << mBox.y << ',' << mBox.width << "," << mBox.height << endl;
		mX = Parameters::convWidthPixToCm(mBox.x);
		mY = Parameters::convHeightPixToCm(mBox.y);
	}
}
