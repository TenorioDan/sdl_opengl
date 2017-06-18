#pragma once
#include "GameObject.h"
#include "SpriteSheet.h"
#include <string>

class AnimatedGameObject : public GameObject
{
public:
	AnimatedGameObject();
	virtual void update(int time);
	virtual void render();

protected:
	int previousAnimationTime;
	int currentAnimationTime;
	int animationSpeed; // time between animations in miliseconds
	int startAnimationIndex;
	int endAnimationIndex;

	SpriteSheet* spriteSheet;
};