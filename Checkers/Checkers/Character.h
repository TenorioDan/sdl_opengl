#pragma once
#include "SpriteSheet.h"

class Character
{
public:
	Character();
	~Character();
	
	void update(int time);
	void render();
	bool loadMedia();

	int PositionX();
	int PositionY();
	
	int MoveSpeed();
	void setMoveSpeed(int newSpeed);
	void moveX(int displacement);
	void moveY(int displacement);

protected:
	SpriteSheet characterSpriteSheet;
	int positionX;
	int positionY;
	int totalSprites;
	int spriteIndex;
	int moveSpeed;
	int previousAnimationTime;
	int currentAnimationTime;
	int animationSpeed; // time between animations in miliseconds

};