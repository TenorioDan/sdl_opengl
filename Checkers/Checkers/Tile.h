#pragma once
#include "Texture.h"

class Tile
{
public:
	Tile(Texture& t, int x, int y);
	void render(SDL_Renderer* renderer);

private:
	Texture texture;
	int positionX;
	int positionY;
};