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
	void applyVerticalAimDirection(GLfloat directionModifier);
	void releaseVerticalAimDirection();
	void reduceHorizontalMovement();
	void setMoveSpeed(int newSpeed);
	void resetPosition();
	void jump();
	void resetJump();
	void attack();
	void aim();
	void stopAiming();

	GLfloat MoveSpeed();

protected:
	GLfloat baseJumpSpeed = -48.f; // move upwards which is the negative y direction
	GLfloat colliderOffset = 30.f;
	GLfloat aimDirectionX;
	GLfloat aimDirectionY;

	// Protected primitives
	int jumpAnimationSpeed;
	bool canJump;
	bool aiming;

	GLfloat moveSpeed;

	// Protected object member variables
	
	Weapon weapon;
	ChracterState currentState;

	void checkCollisions();
};