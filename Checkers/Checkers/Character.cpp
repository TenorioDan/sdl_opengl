#include "Character.h"

Character::Character()
	: GameObject()
{
	positionX = 128;
	positionY = 450;
	width = 64;
	height = 128;
	spriteIndex = 0;
	totalSprites = 0;
	moveSpeed = 2;
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
	if (!characterSpriteSheet.loadTextureFromFileWithColorKey("character_test.png", 255, 0, 255))
	{
		printf("Unable to load sprite sheet!\n");
		return false;
	}

	// set clips
	LFRect clip = { 0.f, 0.f, 64.f, 128.f };

	// Top left
	clip.x = 0.f;
	clip.y = 0.f;
	characterSpriteSheet.addClipSprite(clip);

	// Top right
	clip.x = 64.f;
	clip.y = 0.f;
	characterSpriteSheet.addClipSprite(clip);

	// Bottom left
	clip.x = 128.f;
	clip.y = 0.f;
	characterSpriteSheet.addClipSprite(clip);

	// Bottom right
	clip.x = 192.f;
	clip.y = 0.f;
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

	if (currentState == JUMPING)
	{
		applyGravity();
	}

	if (time - currentAnimationTime >= animationSpeed)
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
	}
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