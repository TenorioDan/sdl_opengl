#pragma once

#include <string>
#include <vector>
#include "Enemy.h"
#include "Camera.h"
#include "Character.h"
#include "Command.h"

class LevelManager
{
public:

	static LevelManager* getInstance();
	~LevelManager();

	void render();
	void update(int time);
	
	void setCameraPosition(GLfloat offsetX, GLfloat offsetY);
	void executeCommand(Command* command);
	void buildWorld(std::string path);
	std::vector<Enemy*>* getEnemies();
	std::vector<Collider*>* getPlatforms();

private:
	// Singleton privates
	static LevelManager* instance;
	LevelManager();

	Camera camera;
	Character player;
	std::vector<Enemy*> enemies;

	// brought over from the tile manager class that i had before
	enum ReadState
	{
		Tiles
		, Colliders
		, Enemies
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

	SpriteSheet* tileSheet;
	Tile** tileset;
	std::vector<Collider*> platforms;

	void clearTiles();
	//void createColliders(std::string path);
	void createTile(int row, int column, int tileType);
	void createCollider(int minX, int minY, int maxX, int maxY);
	void spawnEnemy(std::vector<std::string>::iterator it);
	void renderTileset();
	
};