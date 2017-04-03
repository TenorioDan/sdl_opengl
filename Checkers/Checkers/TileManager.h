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

	const GLfloat tileWidth = 64.f;

	GLint numTilesX;
	GLint numTilesY;
	GLfloat tileSpriteOffeset = 10;

	// Singleton stuff
	TileManager();
	TileManager(TileManager const&);
	//TileManager& operator=(TileManager const&);
	static TileManager* instance;

	SpriteSheet tileSheet;
	Tile** tileset;
	std::vector<Collider*> platforms;

	void clearTiles();
	void createTileset(GLint tilesX, GLint tilesY, std::string path);
	void createColliders(std::string path);
	
};
