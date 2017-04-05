#pragma once
#include "SpriteSheet.h"
#include "GameObject.h"
#include "Projectile.h"
#include <vector>

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

	void createAnimations(SpriteSheet& spritesheet, GLfloat spriteWidth, GLfloat spriteHeight, GLfloat spriteOffset,
		GLfloat spriteStartPositionX, GLfloat spriteStartPositionY, int animationCount);
	void applyHorizontalMovement(GLfloat directionModifier);
	void reduceHorizontalMovement();
	void setMoveSpeed(int newSpeed);
	void resetPosition();
	void jump();
	void resetJump();
	void attack();

	GLfloat MoveSpeed();

protected:
	// Protected primitives
	int jumpAnimationSpeed;
	bool canJump;

	GLfloat moveSpeed;

	const GLfloat baseJumpSpeed = -44.f; // move upwards which is the negative y direction
	const GLfloat colliderOffset = 30.f;	

	// Protected object member variables
	SpriteSheet characterSpriteSheet;
	SpriteSheet projectileSpriteSheet;

	std::vector<Projectile*> projectiles;
		
	ChracterState currentState;

	void checkCollisions();
};