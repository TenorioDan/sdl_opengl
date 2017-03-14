#pragma once
#include "BoardSpace.h"
#include <SDL.h>
#include <SDL_image.h>

class Board
{
public:
	Board();
	// These textures need to by passed by reference. Why? Stackoverflow says something about raw pointers being copied because they are 
	// destroyed after?
	// TODO: Research more about this shit
	Board(Texture& whiteTexture, Texture& greenTexture);
	void render(SDL_Renderer* renderer);
	int getBoardSize();
private:
	int boardRows = 8;
	int boardColumns = 8;
	BoardSpace* boardSpaces[8][8];
	
};