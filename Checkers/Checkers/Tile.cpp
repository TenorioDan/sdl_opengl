#include "Tile.h"

Tile::Tile(Texture& t, int x, int y)
{
	texture = t;
	positionX = x;
	positionY = y;
}

void Tile::render(SDL_Renderer* renderer)
{
	texture.render(renderer, positionX, positionY);
}