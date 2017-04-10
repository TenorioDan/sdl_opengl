#include "Character.h"
#include "LevelManager.h"
#include <cmath>

Character::Character()
	: AnimatedGameObject()
{
	// TODO: Clean up magic numbers
	baseJumpSpeed = -32.f; // move upwards which is the negative y direction
	colliderOffset = 30.f;

	health = 100;
	positionX = 128;
	positionY = 0;
	width = 85.f;
	height = 87.f;
	spriteIndex = 0;
	moveSpeed = 10;
	previousAnimationTime = 0;
	currentAnimationTime = 0;
	animationSpeed = 100;
	jumpAnimationSpeed = 50;
	currentState = IDLE;
	verticalVelocity = 0;

	useGravity = true;
	canJump = true;
	aiming = false;

	direction = RIGHT;
}

Character::~Character()
{
	spriteSheet->freeSheet();
}

void Character::translate(GLfloat x, GLfloat y)
{
	positionX += x;
	positionY += y;

	collider.prevMaxX = collider.maxX;
	collider.prevMaxY = collider.maxY;
	collider.prevMinX = collider.minX;
	collider.prevMinY = collider.minY;

	collider.maxX = positionX + (width / 2) - colliderOffset;
	collider.maxY = positionY + (height / 2);
	collider.minX = positionX - (width / 2) + colliderOffset;
	collider.minY = positionY - (height / 2);
}

bool Character::loadMedia()
{
	spriteSheet = new SpriteSheet();
	if (!spriteSheet->loadTextureFromFileWithColorKey("shovel_knight_original.png", 255, 255, 255))
	{
		printf("Unable to load sprite sheet!\n");
		return false;
	}

	spriteSheet->createAnimations(width, height, 5.f, 266.f, 10.f, 4);

	if (!spriteSheet->generateDataBuffer())
	{
		printf("Unable to clip sprite sheet!\n");
		return false;
	}

	if (!weapon.loadMedia())
	{
		printf("Unable to load weapon media!\n");
		return false;
	}

	return true;
}

void Character::attack()
{
	weapon.fireWeapon(positionX, positionY, direction, aimDirectionX, aimDirectionY, platforms, enemies);
}

void Character::jump()
{
	if (verticalPhysicsState == AT_REST && canJump && canMoveVertical && !aiming)
	{
		verticalPhysicsState = IN_MOTION;
		verticalVelocity = baseJumpSpeed;
		currentPlatform = NULL;
		canJump = false;
	}
}

// TODO: Change collision results based on object type
// TODO: Quad Trees for per section collision detection
void Character::checkCollisions()
{
	if (currentPlatform != NULL && collider.collision(*currentPlatform) == Collider::CollisionDirection::NO_COLLISION)
	{
		currentPlatform = NULL;
		verticalPhysicsState = IN_MOTION;
		verticalVelocity = 0.f;
	}

	// check against platforms that 
	for (auto p : *platforms)
	{
		// Predictive collision detection
		translate(horizontalVelocity, verticalVelocity);

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
			verticalVelocity = 0;
			break;
		case Collider::CollisionDirection::BELOW:
			verticalVelocity = 0;
			break;
		}

		translate(-horizontalVelocity, -verticalVelocity);
	}
}


void Character::resetJump()
{
	canJump = true;
}

void Character::applyHorizontalMovement(GLfloat directionModifier)
{
	aimDirectionX = directionModifier;

	if (canMoveHorizontal)
	{
		horizontalVelocity = (moveSpeed * directionModifier);
		horizontalPhysicsState = IN_MOTION;

		// TODO: Clean up magic numbers
		if (directionModifier > 0)
		{
			direction = RIGHT;
			startAnimationIndex = 0;
			endAnimationIndex = 3;
		}
		else if (directionModifier < 0)
		{
			direction = LEFT;
			startAnimationIndex = 4;
			endAnimationIndex = 7;
		}
	}
}

void Character::reduceHorizontalMovement()
{
	aimDirectionX = 0.f;
	horizontalVelocity = 0.f;
	horizontalPhysicsState = AT_REST;

}

void Character::applyVerticalAimDirection(GLfloat directionModifier)
{
	if (directionModifier < 0 && !aiming)
	{
		// aiming lower based on crouch logic
	}
	else
	{
		aimDirectionY = directionModifier;
	}
}

void Character::releaseVerticalAimDirection()
{
	aimDirectionY = 0.f;
}

GLfloat Character::MoveSpeed()
{
	return moveSpeed;
}

void Character::setMoveSpeed(int newSpeed)
{
	moveSpeed = newSpeed;
}

void Character::aim()
{
	aiming = true;
	canMoveHorizontal = false;

	// only stop moving if the character is grounded. Otherwise continue
	// applying horizontal velocty
	if (currentPlatform != NULL)
	{
		horizontalVelocity = 0.f;
	}
}

void Character::stopAiming()
{
	aiming = false;
	canMoveHorizontal = true;
}

// For debugging
void Character::resetPosition()
{
	positionX = 128;
	positionY = 0;
}

void Character::setEnemies(std::vector<Enemy*>* e)
{
	enemies = e;
}

// Virtual character update that switches between character states
// applies gravity and swaps animations 
void Character::update(int time)
{
	AnimatedGameObject::update(time);
	weapon.update(time);
}

void Character::render()
{
	AnimatedGameObject::render();
	weapon.render();
}