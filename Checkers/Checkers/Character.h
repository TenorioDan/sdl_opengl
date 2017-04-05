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

	enum Direction
	{
		LEFT,
		RIGHT
	};

	Character();
	~Character();
	bool loadMedia();
	
	virtual void update(int time);
	virtual void render();
	virtual void translate(GLfloat x, GLfloat y) override;

	void createAnimations(SpriteSheet& spritesheet);
	void applyHorizontalMovement(GLfloat directionModifier);
	void reduceHorizontalMovement();
	void setMoveSpeed(int newSpeed);
	void resetPosition();
	void jump();
	void resetJump();

	GLfloat MoveSpeed();

protected:
	// Protected primitives
	int previousAnimationTime;
	int currentAnimationTime;
	int animationSpeed; // time between animations in miliseconds
	int jumpAnimationSpeed;
	int startAnimationIndex;
	int endAnimationIndex;
	bool canJump;

	GLuint spriteIndex;
	GLfloat moveSpeed;

	const GLfloat baseJumpSpeed = -44.f; // move upwards which is the negative y direction
	const GLfloat colliderOffset = 30.f;	

	// Protected object member variables
	SpriteSheet characterSpriteSheet;
	ChracterState currentState;
	Direction direction;

};