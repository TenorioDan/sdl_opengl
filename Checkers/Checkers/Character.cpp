#include "Character.h"
#include <cmath>

Character::Character()
	: GameObject()
{
	positionX = 128;
	positionY = 512;
	width = 85.f;
	height = 87.f;
	spriteIndex = 0;
	totalSprites = 0;
	moveSpeed = 4;
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

	translate(horizontalVelocity, 0.f);

	if (time - currentAnimationTime >= animationSpeed && horizontalVelocity != 0 && currentPhysicsState != FALLING)
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
	if (currentPhysicsState != FALLING)
	{
		currentPhysicsState = FALLING;
		verticleVelocity = baseJumpSpeed;
		//currentPlatform = NULL;
	}
}

void Character::applyHorizontalMovement(GLfloat directionModifier)
{

	horizontalVelocity = (moveSpeed * directionModifier);
}

void Character::reduceHorizontalMovement()
{
	horizontalVelocity = 0;
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
	positionY = 512;
}