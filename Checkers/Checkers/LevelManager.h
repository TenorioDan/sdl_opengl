#pragma once

#include <string>
#include <vector>
#include "Enemy.h"
#include "Camera.h"
#include "Character.h"
#include "TileManager.h"
#include "Command.h"

class LevelManager
{
public:
	LevelManager();
	~LevelManager();

	void render();
	void update(int time);
	
	bool loadMedia();
	void spawnEnemies(std::string path);
	void setCameraPosition(GLfloat offsetX, GLfloat offsetY);
	void executeCommand(Command* command);
	TileManager getTileManager();

	std::vector<Enemy*> getEnemies();

private:
	Camera camera;
	Character player;
	std::vector<Enemy*> enemies;
	TileManager tileManager;
	
};