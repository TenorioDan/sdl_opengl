#include "Character.h"
#include <cmath>

Character::Character()
	: GameObject()
{
	positionX = 128;
	positionY = 0;
	width = 85.f;
	height = 87.f;
	spriteIndex = 0;
	totalSprites = 0;
	moveSpeed = 8;
	previousAnimationTime = 0;
	currentAnimationTime = 0;
	animationSpeed = 100;
	jumpAnimationSpeed = 50;
	currentState = IDLE;
	verticleVelocity = 0;
	useGravity = true;
}

Character::~Character()
{
	characterSpriteSheet.freeSheet();
}

void Character::translate(GLfloat x, GLfloat y)
{
	positionX += x;
	positionY += y;

	collider.prevMaxX = collider.maxX;
	collider.prevMaxY = collider.maxY;
	collider.prevMinX = collider.minX;
	collider.prevMinY = collider.minY;

	collider.maxX = positionX + (width / 2) - 30;
	collider.maxY = positionY + (height / 2);
	collider.minX = positionX - (width / 2) + 30;
	collider.minY = positionY - (height / 2);
}

bool Character::loadMedia()
{
	if (!characterSpriteSheet.loadTextureFromFileWithColorKey("shovel_knight_original.png", 255, 255, 255))
	{
		printf("Unable to load sprite sheet!\n");
		return false;
	}

	// set clips
	LFRect clip = { 0.f, 0.f, 85.f, 87.f };
	int offset = 5;

	// Top left
	clip.x = 266.f;
	clip.y = 10.f;
	characterSpriteSheet.addClipSprite(clip);

	// Top right
	clip.x = 266.f + (85 * 1) + (offset * 1);
	clip.y = 10.f;
	characterSpriteSheet.addClipSprite(clip);

	// Bottom left
	clip.x = 266.f + (85 * 2) + (offset * 2);
	clip.y = 10.f;
	characterSpriteSheet.addClipSprite(clip);

	// Bottom right
	clip.x = 266.f + (85 * 3) + (offset * 3);
	clip.y = 10.f;
	characterSpriteSheet.addClipSprite(clip);

	if (!characterSpriteSheet.generateDataBuffer())
	{
		printf("Unable to clip sprite sheet!\n");
		return false;
	}

	totalSprites = 4;

	return true;
}


// Virtual character update that switches between character states
// applies gravity and swaps animations 
void Character::update(int time)
{
	GameObject::update(time);

	if (time - currentAnimationTime >= animationSpeed && horizontalPhysicsState == IN_MOTION)
	{
		previousAnimationTime = currentAnimationTime;
		currentAnimationTime = time;

		spriteIndex++;

		if (spriteIndex >= totalSprites)
		{
			spriteIndex = 0;
		}
	}
}

void Character::jump()
{
	if (verticalPhysicsState == AT_REST)
	{
		verticalPhysicsState = IN_MOTION;
		verticleVelocity = baseJumpSpeed;
		currentPlatform = NULL;
	}
}

void Character::applyHorizontalMovement(GLfloat directionModifier)
{

	horizontalVelocity = (moveSpeed * directionModifier);
	horizontalPhysicsState = IN_MOTION;
}

void Character::reduceHorizontalMovement()
{
	horizontalVelocity = 0;
	horizontalPhysicsState = AT_REST;

}


void Character::render()
{
	GameObject::render();
	characterSpriteSheet.renderSprite(spriteIndex);
}


GLfloat Character::MoveSpeed()
{
	return moveSpeed;
}

void Character::setMoveSpeed(int newSpeed)
{
	moveSpeed = newSpeed;
}

void Character::resetPosition()
{
	positionX = 128;
	positionY = 0;
}