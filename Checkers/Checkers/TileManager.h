#pragma once

#include <vector>
#include "SpriteSheet.h"
#include "Collider.h"

class TileManager
{
public:
	// Singleton stuff
	TileManager();
	std::vector<Collider*>* getPlatforms();

	bool loadMedia();
	void render();

private:
	enum ReadState
	{
		Tiles
		,Colliders
		,Enemies
	};
	
	struct Tile
	{
		unsigned int spriteIndex;
		GLfloat positionX;
		GLfloat positionY;
	};

	const GLfloat tileWidth = 64.f;
	const GLfloat tileHeight = 64.f;

	GLint numTilesX;
	GLint numTilesY;
	GLfloat tileSpriteOffeset = 10;
	ReadState readState;

	SpriteSheet tileSheet;
	Tile** tileset;
	std::vector<Collider*> platforms;

	void clearTiles();
	void createTileset(std::string path);
	void createColliders(std::string path);
	void createTile(int row, int column, int tileType);
	void createCollider(int minX, int minY, int maxX, int maxY);
	std::string findMostRecentLevelFile();
	
};
