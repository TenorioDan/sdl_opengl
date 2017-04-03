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
	bool loadMedia();
	
	virtual void update(int time);
	virtual void render();
	virtual void translate(GLfloat x, GLfloat y) override;

	void applyHorizontalMovement(GLfloat directionModifier);
	void reduceHorizontalMovement();
	void setMoveSpeed(int newSpeed);
	void resetPosition();
	void jump();
	void resetJump();

	GLfloat MoveSpeed();

protected:
	int previousAnimationTime;
	int currentAnimationTime;
	int animationSpeed; // time between animations in miliseconds
	int jumpAnimationSpeed;
	bool canJump;

	// Protected primitives
	GLuint totalSprites;
	GLuint spriteIndex;
	GLfloat moveSpeed;

	const GLfloat baseJumpSpeed = -44.f; // move upwards which is the negative y direction
	const GLfloat colliderOffset = 30.f;	

	// Protected object member variables
	SpriteSheet characterSpriteSheet;
	ChracterState currentState;

};