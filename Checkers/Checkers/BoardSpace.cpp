#include "BoardSpace.h"
BoardSpace::BoardSpace()
{
	this->positionX = 0;
	this->positionY = 0;
}

BoardSpace::BoardSpace(Texture& texture, int positionX, int positionY)
{
	this->texture = texture;
	this->positionX = positionX;
	this->positionY = positionY;
}

void BoardSpace::render(SDL_Renderer* renderer)
{
	texture.render(renderer, positionX, positionY);
}