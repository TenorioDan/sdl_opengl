#include "GameObject.h"
#include "TileManager.h"

GameObject::GameObject()
{
	verticleVelocity = 0.f;
	positionX = 0;
	positionY = 0;
	verticleVelocity = 0;
	verticalPhysicsState = IN_MOTION;
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

	collider.prevMaxX = collider.maxX;
	collider.prevMaxY = collider.maxY;
	collider.prevMinX = collider.minX;
	collider.prevMinY = collider.minY;

	collider.maxX = positionX + (width / 2);
	collider.maxY = positionY + (height / 2);
	collider.minX = positionX - (width / 2);
	collider.minY = positionY - (height / 2);
}

// TODO: Change collision results based on object type
// TODO: Quad Trees for per section collision detection
void GameObject::checkCollisions()
{
	std::vector<Collider*> platforms = TileManager::getInstance()->getPlatforms();

	if (currentPlatform != NULL && collider.collision(*currentPlatform) == Collider::CollisionDirection::NO_COLLISION)
	{
		currentPlatform = NULL;
		verticalPhysicsState = IN_MOTION;
		verticleVelocity = 0.f;
	}

	// check against platforms that 
	for (auto p : platforms)
	{
		// Predictive collision detection
		translate(horizontalVelocity, verticleVelocity);

		switch (collider.collision(*p))
		{
		case Collider::CollisionDirection::LEFT:
			translate(-horizontalVelocity, 0.f);
			break;
		case Collider::CollisionDirection::RIGHT:
			translate(-horizontalVelocity, 0.f);
			break;
		case Collider::CollisionDirection::ABOVE:
			currentPlatform = p;
			positionY = p->minY - (height / 2.f);
			verticalPhysicsState = AT_REST;
			verticleVelocity = 0;
			break;
		case Collider::CollisionDirection::BELOW:
			verticleVelocity = 0;
			break;
		}

		translate(-horizontalVelocity, -verticleVelocity);
	}
}

void GameObject::applyGravity()
{
	if (useGravity && verticalPhysicsState == IN_MOTION && verticleVelocity < maxVerticleSpeed)
	{
		verticleVelocity += gravity;
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

void GameObject::update(int time)
{
	applyGravity();
	checkCollisions();
	translate(horizontalVelocity, verticleVelocity);
}

void GameObject::render()
{
	//glLoadIdentity();
	glTranslatef(positionX, positionY, 0.f);
}