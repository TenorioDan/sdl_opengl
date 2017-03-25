#include "GameObject.h"

GameObject::GameObject()
{
	verticleVelocity = 0.f;
	positionX = 0;
	positionY = 0;
	verticleVelocity = 0;
	currentPhysicsState = AT_REST;
}

Collider GameObject::getCollider()
{
	return collider;
}


void GameObject::translate(GLfloat x, GLfloat y)
{
	positionX += x;
	positionY += y;
}

void GameObject::applyGravity()
{
	if (currentPhysicsState == FALLING && useGravity)
	{
		translate(0.f, verticleVelocity);

		if (verticleVelocity > 0)
		{
			// check against platforms
		}

		if (verticleVelocity < maxVerticleSpeed)
		{
			verticleVelocity += 5.f;
		}
	}
}

GameObject::PhysicsState GameObject::getPhysicsState()
{
	return currentPhysicsState;
}

GLfloat GameObject::PositionX()
{
	return positionX;
}

GLfloat GameObject::PositionY()
{
	return positionY;
}


void GameObject::update(int time)
{
	applyGravity();
}

void GameObject::render()
{
	glLoadIdentity();
	glTranslatef(positionX, positionY, 0.f);
}