#pragma once
#include "AnimatedGameObject.h"
#include "Weapon.h"
#include "Enemy.h"

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
	void detectPlatformCollision(Collider* c, Collider::CollisionDirection d);
	void releaseVerticalAimDirection();
	void reduceHorizontalMovement();
	void setMoveSpeed(int newSpeed);
	void resetPosition();
	void jump();
	void resetJump();
	void attack();
	void aim();
	void stopAiming();
	void setEnemies(std::vector<Enemy*>* e);

	GLfloat MoveSpeed();

protected:
	GLfloat baseJumpSpeed = -48.f; // move upwards which is the negative y direction
	GLfloat colliderOffset = 30.f;
	GLfloat aimDirectionX;
	GLfloat aimDirectionY;
	GLfloat health;

	// Protected primitives
	int jumpAnimationSpeed;
	bool canJump;
	bool aiming;

	GLfloat moveSpeed;

	// Protected object member variables
	
	Weapon weapon;
	ChracterState currentState;
	std::vector<Enemy*>* enemies;

	void checkCollisions();
};