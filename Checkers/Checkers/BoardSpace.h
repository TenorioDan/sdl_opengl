#pragma once

#include "Texture.h"


class BoardSpace 
{
public:
	BoardSpace();
	BoardSpace(Texture& text, int positionX, int positionY);
	void render(SDL_Renderer* renderer);
private:
	Texture texture;
	int positionX;
	int positionY;
};