#pragma once

#include "SpriteSheet.h"

class TileManager
{
public:
	struct Tile
	{
		unsigned int spriteIndex;
		GLfloat positionX;
		GLfloat positionY;
	};

	TileManager();
	bool loadMedia();
	void render();

private:
	SpriteSheet tileSheet;
	int spriteOffeset;
	Tile* tileset[40][23];
	
};
