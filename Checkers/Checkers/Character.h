#pragma once
#include "SpriteSheet.h"
#include "GameObject.h"

class Character : public GameObject
{
public:
	
	enum ChracterState
	{
		IDLE,
		RUNNING,
		JUMPING
	};

	Character();
	~Character();
	
	virtual void update(int time);
	virtual void render();
	bool loadMedia();

	GLfloat MoveSpeed();

	void setMoveSpeed(int newSpeed);
	void jump();
	

protected:
	// Protected primitives
	GLuint totalSprites;
	GLuint spriteIndex;
	GLfloat moveSpeed;

	const GLfloat baseJumpSpeed = -52.f; // move upwards which is the negative y direction

	int previousAnimationTime;
	int currentAnimationTime;
	int animationSpeed; // time between animations in miliseconds
	int jumpAnimationSpeed;

	// Protected object member variables
	SpriteSheet characterSpriteSheet;
	ChracterState currentState;

};