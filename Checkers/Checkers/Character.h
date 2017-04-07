#pragma once
#include "AnimatedGameObject.h"
#include "Weapon.h"

class Character : public AnimatedGameObject
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
	void attack();

	GLfloat MoveSpeed();

protected:
	const GLfloat baseJumpSpeed = -44.f; // move upwards which is the negative y direction
	const GLfloat colliderOffset = 30.f;

	// Protected primitives
	int jumpAnimationSpeed;
	bool canJump;

	GLfloat moveSpeed;

	// Protected object member variables
	SpriteSheet characterSpriteSheet;
	Weapon weapon;
	ChracterState currentState;

	void checkCollisions();
};