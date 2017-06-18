#include "Character.h"
#include "LevelManager.h"
#include "MediaManager.h"
#include <cmath>

Character::Character()
	: AnimatedGameObject()
{
	// TODO: Clean up magic numbers
	baseJumpSpeed = -28.f; // move upwards which is the negative y direction
	colliderOffset = 30.f;

	health = 100;
	positionX = 650;
	positionY = 720;
	width = 85.f;
	height = 87.f;
	spriteIndex = 0;
	moveSpeed = 12;
	previousAnimationTime = 0;
	currentAnimationTime = 0;
	animationSpeed = 100;
	jumpAnimationSpeed = 50;
	currentState = IDLE;
	verticalVelocity = 0;
	currentJumpFrameCount = maxJumpFrameCount = 2;
	colliderOffset = 30.f;

	useGravity = true;
	canJump = true;
	aiming = false;

	direction = RIGHT;
	spriteSheet = MediaManager::getInstance()->getSpriteSheet("MAIN_CHARACTER");
}

Character::~Character()
{
	spriteSheet->freeSheet();
}


void Character::attack()
{
	weapon.fireWeapon(positionX, positionY, direction, aimDirectionX, aimDirectionY, enemies);
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

void Character::resetJump()
{
	canJump = true;

	if (verticalVelocity < 0)
	{
		verticalVelocity /= 2;
	}
}

void Character::jumpUpdate()
{/*
	if (isJumping && !aiming && verticalVelocity > baseJumpSpeed)
	{
		if (currentJumpFrameCount >= maxJumpFrameCount)
		{
			currentJumpFrameCount = 0;
			verticalPhysicsState = IN_MOTION;
			verticalVelocity -=12;
			currentPlatform = NULL;
		}
		else
		{
			++currentJumpFrameCount;
		}
	}
	else
	{
		currentJumpFrameCount = maxJumpFrameCount;
		isJumping = false;
	}*/
}

void Character::checkCollisions()
{
	if (currentPlatform != NULL && collider.collision(*currentPlatform) == Collider::CollisionDirection::NO_COLLISION)
	{
		currentPlatform = NULL;
		verticalPhysicsState = IN_MOTION;
		verticalVelocity = 0.f;
	}

	LevelManager* levelManager = LevelManager::getInstance();

	for (auto p : *levelManager->getPlatforms())
	{
		Collider::CollisionDirection collision = collider.collision(*p);

		if (collision != Collider::NO_COLLISION)
		{
			detectPlatformCollision(p, collision);
		}
	}
}

// Called by the level manager when the player has fallen on a new platform
void Character::detectPlatformCollision(Collider* platform, Collider::CollisionDirection direction)
{
	switch (direction)
	{
	case Collider::CollisionDirection::LEFT:
		positionX -= horizontalVelocity;
		break;
	case Collider::CollisionDirection::RIGHT:
		positionX -= horizontalVelocity;
		break;
	case Collider::CollisionDirection::ABOVE:
		currentPlatform = platform;
		positionY = platform->minY - (height / 2.f);
		verticalPhysicsState = AT_REST;
		verticalVelocity = 0;
		break;
	case Collider::CollisionDirection::BELOW:
		verticalVelocity = 0;
		canJump = true;
		break;
	}
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
	positionX = 650;
	positionY = 720;
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
	jumpUpdate();
	weapon.update(time);
}

void Character::render()
{
	AnimatedGameObject::render();
	weapon.render();
}