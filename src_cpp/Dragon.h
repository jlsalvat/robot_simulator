#pragma once

#include "SpriteAnimated.h"

enum class Sens {
	left, right, up, down
};

class Dragon {
public:
	Dragon(Graphics& graphics, int x, int y);
	void move(Sens sens, int angle);
	void idle();
	void jump();
	void update();
	void draw(Graphics& graphics);
	const Rectangle getRectangle() const { return mBox; }
	void reset();
	void blink(int freq);
	void setAlpha(int alpha);
	void handleTileCollisions(std::vector<Rectangle*> &others);
	bool isOutOfScreen() { return (mBox.y > Parameters::SCREEN_HEIGHT ); }
	void dead() {
		number_of_life--;
		if (number_of_life > 0) {
			reset();
		}
		if (number_of_life < 0)
			number_of_life = 0;
	}
	const int getLife() const {return number_of_life;}
	void returnToLife( )   {  number_of_life=3; }
private:
	int number_of_life = 3;
	void updateBox();
	void start();
	enum state { running, waiting, jumping_idle, jumping_running };
	state mDragon_state=waiting;
	SpriteAnimated mIdle;
	SpriteAnimated mMove;
	SpriteAnimated mJump;
	// pour la gestion des collisions
	Rectangle mBox;
	Sens mSens;
	int mAngle=0;
	int mOffset=0;
	bool mGrounded = false;
	SDL_RendererFlip mFlip=SDL_FLIP_NONE;
	float mX, mY;
	float initial_positionX, initial_positionY;
	int time = 0;
	float x0=0, y0=0;
	static constexpr float GRAVITY = 60.81;
	static constexpr float V0 = 30.0; 
};
