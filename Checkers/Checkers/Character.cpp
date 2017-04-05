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
	moveSpeed = 8;
	previousAnimationTime = 0;
	currentAnimationTime = 0;
	animationSpeed = 100;
	jumpAnimationSpeed = 50;
	currentState = IDLE;
	verticleVelocity = 0;
	useGravity = true;
	canJump = true;
	direction = RIGHT;
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

	collider.maxX = positionX + (width / 2) - colliderOffset;
	collider.maxY = positionY + (height / 2);
	collider.minX = positionX - (width / 2) + colliderOffset;
	collider.minY = positionY - (height / 2);
}

bool Character::loadMedia()
{
	if (!characterSpriteSheet.loadTextureFromFileWithColorKey("shovel_knight_original.png", 255, 255, 255))
	{
		printf("Unable to load sprite sheet!\n");
		return false;
	}

	createAnimations(characterSpriteSheet);

	if (!characterSpriteSheet.generateDataBuffer())
	{
		printf("Unable to clip sprite sheet!\n");
		return false;
	}

	return true;
}

// Create the character animations based on the spritesheets provided
void Character::createAnimations(SpriteSheet& spritesheet)
{
	// TODO: Create character spritesheet and define locations
	// set clips
	LFRect clip = { 0.f, 0.f, width, height };
	GLfloat offset = 5.f;
	GLfloat startPositionX = 266.f;

	// Add running right sprites
	for (int i = 0; i < 4; ++i)
	{
		clip.x = startPositionX + (width * i) + (offset * i);
		clip.y = 10.f;
		spritesheet.addClipSprite(clip);
	}
	
	for (int i = 0; i < 4; ++i)
	{
		clip.x = spritesheet.imageWidth() - (startPositionX + (width * (i + 1)) + (offset * i));
		clip.y = 10.f;
		spritesheet.addClipSprite(clip);
	}
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

		if (spriteIndex < startAnimationIndex || spriteIndex > endAnimationIndex)
		{
			spriteIndex = startAnimationIndex;
		}
	}
}

void Character::jump()
{
	if (verticalPhysicsState == AT_REST && canJump)
	{
		verticalPhysicsState = IN_MOTION;
		verticleVelocity = baseJumpSpeed;
		currentPlatform = NULL;
		canJump = false;
	}
}

void Character::resetJump()
{
	canJump = true;
}

void Character::applyHorizontalMovement(GLfloat directionModifier)
{

	horizontalVelocity = (moveSpeed * directionModifier);
	horizontalPhysicsState = IN_MOTION;

	if (directionModifier >= 0)
	{
		direction = RIGHT;
		startAnimationIndex = 0;
		endAnimationIndex = 3;
	}
	else
	{
		direction = LEFT;
		startAnimationIndex = 4;
		endAnimationIndex = 7;
	}
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
	glTranslatef(-positionX, -positionY, 0.f);
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