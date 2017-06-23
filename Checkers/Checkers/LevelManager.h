#pragma once

#include <string>
#include <vector>
#include <map>
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
	void checkTransitions();

	void setCameraPosition(GLfloat offsetX, GLfloat offsetY);
	void executeCommand(Command* command);
	void buildLevel(std::string path);
	void transitionToLevel(std::string transitionLevelName, std::string transitionTileName, Collider::CollisionDirection direction, GLfloat previousY);
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
		, Transitions
	};

	struct Tile
	{
		unsigned int spriteIndex;
		GLfloat positionX;
		GLfloat positionY;
	};

	struct Transition
	{
		std::string transitionLevelName;
		std::string transitionTileName;
		Collider collider;
	};

	const GLfloat tileWidth = 64.f;
	const GLfloat tileHeight = 64.f;

	GLint tileCount;
	GLint currentTile = 0;
	GLfloat transitionOffset;
	GLfloat tileSpriteOffeset = 10;
	ReadState readState;

	SpriteSheet* tileSheet;
	Tile** tileset;
	std::vector<Collider*> platforms;
	std::map<std::string, Transition> transitions;

	// Private updates
	void updateEnemies(int time);
	

	void clearTiles();
	void clearEnemies();
	//void createColliders(std::string path);
	void createTile(std::vector<std::string>::iterator it);
	void createCollider(std::vector<std::string>::iterator it);
	void spawnEnemy(std::vector<std::string>::iterator it);
	void createTransition(std::vector<std::string>::iterator it);
	void renderTileset();
	
};