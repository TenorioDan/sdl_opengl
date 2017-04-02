#pragma once

#include <vector>
#include "SpriteSheet.h"
#include "Collider.h"

class TileManager
{
public:
	static TileManager* getInstance();

	std::vector<Collider*> getPlatforms();

	bool loadMedia();
	void render();

private:
	struct Tile
	{
		unsigned int spriteIndex;
		GLfloat positionX;
		GLfloat positionY;
	};

	// Singleton stuff
	TileManager();
	TileManager(TileManager const&);
	//TileManager& operator=(TileManager const&);
	static TileManager* instance;

	SpriteSheet tileSheet;
	Tile* tileset[40][40];
	std::vector<Collider*> platforms;
	
};
