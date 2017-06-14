#pragma once

#include "Collider.h"
#include <vector>

class GameObject
{
public:
	enum PhysicsState
	{
		AT_REST,
		IN_MOTION,
	};

	enum Direction
	{
		LEFT,
		RIGHT
	};

	GameObject();

	virtual void update(int time);
	virtual void render();

	virtual void translate(GLfloat x, GLfloat y);
	
	//void applyHorizontalVelocity;
	Collider getCollider();

	GLfloat PositionX();
	GLfloat PositionY();
	GLfloat getHorizontalVelocity();
	GLfloat getVerticalVelocity();
	void setVerticalVelocity(GLfloat v);
	void setHorizontalVelocity(GLfloat v);
	void setPosition(GLfloat x, GLfloat y);
	void setPlatforms(std::vector<Collider*>* c);
	bool ShouldBeDeleted();

protected:
	int currentTime = 0;
	int previousTime = 0;
	bool useGravity;
	bool canMoveHorizontal;
	bool canMoveVertical;
	bool toDelete;

	GLuint spriteIndex;
	GLfloat positionX;
	GLfloat positionY;
	GLfloat width;
	GLfloat height;
	GLfloat verticalVelocity;
	GLfloat horizontalVelocity;

	GLfloat gravity;
	GLfloat maxVerticleSpeed; // TODO figure out what max positive verticle velocity should be

	Collider collider;
	Collider* currentPlatform;

	// Protected methods
	virtual void checkCollisions() = 0;
	void applyGravity();

	// Enums
	PhysicsState horizontalPhysicsState;
	PhysicsState verticalPhysicsState;
	Direction direction;

	std::vector<Collider*>* platforms;
};