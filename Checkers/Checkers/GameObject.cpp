#include "GameObject.h"

GameObject::GameObject()
{
	gravity = 2.f;
	maxVerticleSpeed = 24.f;

	verticalVelocity = 0.f;
	horizontalVelocity = 0.f;
	positionX = 0;
	positionY = 0;
	width = 0;
	height = 0;
	verticalPhysicsState = IN_MOTION;

	canMove = true;

	// Animations
	spriteIndex = 0;
}

Collider GameObject::getCollider()
{
	return collider;
}


void GameObject::translate(GLfloat x, GLfloat y)
{
	positionX += x;
	positionY += y;

	collider.prevMaxX = collider.maxX;
	collider.prevMaxY = collider.maxY;
	collider.prevMinX = collider.minX;
	collider.prevMinY = collider.minY;

	collider.maxX = positionX + (width / 2);
	collider.maxY = positionY + (height / 2);
	collider.minX = positionX - (width / 2);
	collider.minY = positionY - (height / 2);
}

void GameObject::applyGravity()
{
	if (useGravity && verticalPhysicsState == IN_MOTION && verticalVelocity < maxVerticleSpeed)
	{
		verticalVelocity += gravity;
	}
}


GLfloat GameObject::PositionX()
{
	return positionX;
}

GLfloat GameObject::PositionY()
{
	return positionY;
}

void GameObject::setVerticalVelocity(GLfloat v)
{
	verticalVelocity = v;
}

void GameObject::setHorizontalVelocity(GLfloat v)
{
	horizontalVelocity = v;
}

void GameObject::update(int time)
{
	applyGravity();
	checkCollisions();

	if (canMove)
	{
		translate(horizontalVelocity, verticalVelocity);
	}
}

void GameObject::render()
{
	//glLoadIdentity();
}