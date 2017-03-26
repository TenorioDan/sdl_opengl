#include "GameObject.h"
#include "TileManager.h"

GameObject::GameObject()
{
	verticleVelocity = 0.f;
	positionX = 0;
	positionY = 0;
	verticleVelocity = 0;
	currentPhysicsState = FALLING;
	landingCollisionNextFrame = false;
}

Collider GameObject::getCollider()
{
	return collider;
}


void GameObject::translate(GLfloat x, GLfloat y)
{
	positionX += x;
	positionY += y;
	collider.setBounds(positionX + (width / 2), positionY + (height / 2 ), positionX - (width / 2), positionY - (height / 2));
}


// Simple algorithm for adding gravity to a gameobject. If the physics state of the object is that it is currently falling,
// update the vertical velocity by the accerlation of gravity. 
// We check collisions against all platforms.
// Update the current y position of the object and then check whether or not the object will collide in the next 
// frame, and keep track of that so that the next time this method is called, the y position of the object is just set to the
// colliding platform.

// TODO: Quad Trees for per section collision detection
void GameObject::applyGravity()
{
	if (currentPhysicsState == FALLING && useGravity)
	{
		if (landingCollisionNextFrame)
		{
			currentPhysicsState = AT_REST;
			landingCollisionNextFrame = false;
			positionY = currentPlatform->MinY() - (height / 2);
		}
		else
		{
			translate(0.f, verticleVelocity);
			verticleVelocity = verticleVelocity >= maxVerticleSpeed ? verticleVelocity : verticleVelocity += 2.f;

			if (verticleVelocity > 0)
			{
				translate(0.f, verticleVelocity);
				std::vector<Collider*> platforms = TileManager::getInstance()->getPlatforms();

				// check against platforms that 
				for (auto p : platforms)
				{
					if (collider.collision(*p))
					{
						landingCollisionNextFrame = true;
						currentPlatform = p;
						break;
					}
				}

				translate(0.f, -verticleVelocity);
			}
		}
	}
	else if (currentPlatform != NULL)
	{
		if (!collider.collision(*currentPlatform))
		{
			currentPhysicsState = FALLING;
			verticleVelocity = 0;
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