#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include "LevelManager.h"

LevelManager::LevelManager() 
{
}

// Destroy all enemies
LevelManager::~LevelManager()
{
	for (auto enemy : enemies)
	{
		delete enemy;
	}
}


// Load media for every object in this level
// TODO: Add path for loading individual levels
bool LevelManager::loadMedia()
{
	bool success = true;

	success = tileManager.loadMedia();
	success = player.loadMedia();

	if (success)
	{
		spawnEnemies("Enemies.txt");
		for (auto enemy : enemies)
		{
			enemy->loadMedia();
		}

		player.setEnemies(&enemies);
		player.setPlatforms(tileManager.getPlatforms());
	}

	return success;
}

void LevelManager::spawnEnemies(std::string path)
{
	std::ifstream enemyFile(path.c_str());
	std::string line;
	std::vector<Collider*> platforms = *tileManager.getPlatforms();

	while (std::getline(enemyFile, line))
	{
		std::istringstream buffer(line);
		std::istream_iterator<std::string> beg(buffer), end;
		std::vector<std::string> values(beg, end);
		auto it = values.begin();

		Enemy* e = new Enemy();
		e->setCurrentPlatform(platforms[std::stoi(*++it)]);
		enemies.push_back(e);
	}
}

// Called by the game to set the position of camera
void LevelManager::setCameraPosition(GLfloat offsetX, GLfloat offsetY)
{
	camera.objectToFollow = dynamic_cast<GameObject*>(&player);
	camera.offsetX = offsetX / 2.f;
	camera.offsetY = offsetY / 1.75f;
}

void LevelManager::executeCommand(Command* command)
{
	command->execute(player);
}

std::vector<Enemy*> LevelManager::getEnemies()
{
	return enemies;
}

TileManager LevelManager::getTileManager()
{
	return tileManager;
}

void LevelManager::update(int time)
{
	player.update(time);

	for (auto it = enemies.begin(); it != enemies.end();)
	{
		Enemy* e = *it;
		e->update(time);

		if (e->ShouldBeDeleted())
		{
			delete e;
			it = enemies.erase(it);
		}
		else
		{
			++it;
		}

	}
}

void LevelManager::render()
{
	camera.render();
	player.render();
	
	for (auto enemy : enemies)
	{
		enemy->render();
	}

	tileManager.render();
}