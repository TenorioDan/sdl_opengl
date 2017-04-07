#pragma once
#include "GameObject.h"

class AnimatedGameObject : public GameObject
{
public:
	AnimatedGameObject();
	void update(int time);

protected:
	int previousAnimationTime;
	int currentAnimationTime;
	int animationSpeed; // time between animations in miliseconds
	int startAnimationIndex;
	int endAnimationIndex;
};