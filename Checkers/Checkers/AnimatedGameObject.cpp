#include "AnimatedGameObject.h"

AnimatedGameObject::AnimatedGameObject()
	: GameObject()
{
	previousAnimationTime = 0;
	currentAnimationTime = 0;
	startAnimationIndex = 0;
	endAnimationIndex = 0;
}

void AnimatedGameObject::update(int time)
{
	GameObject::update(time);

	if (time - currentAnimationTime >= animationSpeed && horizontalPhysicsState == IN_MOTION)
	{
		previousAnimationTime = currentAnimationTime;
		currentAnimationTime = time;

		spriteIndex++;

		if (spriteIndex < startAnimationIndex || spriteIndex > endAnimationIndex)
		{
			spriteIndex = startAnimationIndex;
		}
	}
}