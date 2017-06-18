#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include "LevelManager.h"
#include "MediaManager.h"

LevelManager::LevelManager() { }

LevelManager* LevelManager::instance;

LevelManager* LevelManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new LevelManager();
	}

	return instance;
}

// Destroy all enemies
LevelManager::~LevelManager()
{
	for (auto enemy : enemies)
	{
		delete enemy;
	}

	clearTiles();
}

void LevelManager::buildWorld(std::string path)
{
	tileSheet = MediaManager::getInstance()->getSpriteSheet("TILE_SHEET");
	// dynamically create the number of tiles for this this area
	// Add all tiles that will be rendered to the screen
	std::ifstream  tilesetFile(path.c_str());
	std::string line;

	while (std::getline(tilesetFile, line))
	{
		std::istringstream buffer(line);
		std::istream_iterator<std::string> beg(buffer), end;
		std::vector<std::string> values(beg, end);
		auto it = values.begin();

		if (*it == "TILES")
		{
			readState = Tiles;
			numTilesX = std::stoi(*++it);
			numTilesY = std::stoi(*++it);
			tileset = new Tile*[numTilesX * numTilesY];
		}
		else if (*it == "COLLIDERS")
		{
			readState = Colliders;
		}
		else if (*it == "ENEMIES")
		{
			readState = Enemies;
		}
		else if (*it == "END")
		{
			// Do nothing, we're done reading the file
		}
		else
		{
			switch (readState)
			{
				case Tiles:
				{
					int row = std::stoi(*it++);
					int column = std::stoi(*it++);
					int tileType = std::stoi(*it);
					createTile(row, column, tileType);
					break;
				}
				case Colliders:
				{
					int minX = std::stoi(*it++);
					int minY = std::stoi(*it++);
					int maxX = std::stoi(*it++);
					int maxY = std::stoi(*it);
					createCollider(minX, minY, maxX, maxY);
					break;
				}
				case Enemies:
				{
					spawnEnemy(it);
				}
			}
		}
	}
}

// Build the tile based and add it to the list of current tiles
void LevelManager::createTile(int row, int column, int tileType)
{
	Tile* t = new Tile();
	t->spriteIndex = tileType;
	t->positionX = column * tileWidth;
	t->positionY = row * tileHeight;

	tileset[row*numTilesY + column] = t;
}


void LevelManager::createCollider(int minX, int minY, int maxX, int maxY)
{
	Collider* c = new Collider();
	c->minX = minX - (tileWidth / 2.f);
	c->minY = minY - (tileWidth / 2.f);
	c->maxX = maxX - (tileWidth / 2.f);
	c->maxY = maxY - (tileWidth / 2.f);
	platforms.push_back(c);
}

// Read the enemy file and create enemies in the game
// Pass the iterator instead of the values so that depending on the enemy, we can adjust accordingly
void LevelManager::spawnEnemy(std::vector<std::string>::iterator it)
{
	Enemy* e = new Enemy();
	e->setCurrentPlatform(platforms[std::stoi(*++it)]);
	e->setPosition(std::stoi(*++it), std::stoi(*++it));
	e->setHorizontalVelocity(std::stoi(*++it));
	e->setVerticalVelocity(std::stoi(*++it));
	enemies.push_back(e);
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

std::vector<Collider*>* LevelManager::getPlatforms()
{
	return &platforms;
}

std::vector<Enemy*>* LevelManager::getEnemies()
{
	return &enemies;
}

// Update loop. Check collisions and stuff
void LevelManager::update(int time)
{
	//checkPlayerCollisions();
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

// Render all the tiles in the tileset
void LevelManager::renderTileset()
{
	for (int y = 0; y < numTilesY; ++y)
	{
		for (int x = 0; x < numTilesX; ++x)
		{
			Tile t = *tileset[x * numTilesY + y];

			if (t.spriteIndex > 0)
			{
				// Move to the spot to render and then move back to render the next texture 
				// in the correct spot
				glTranslatef(t.positionX, t.positionY, 0.f);
				tileSheet->renderSprite(t.spriteIndex - 1);
				glTranslatef(-t.positionX, -t.positionY, 0.f);
			}
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

	renderTileset();
}

// Clear the tileset to give room for loading different levels
void LevelManager::clearTiles()
{
	delete[] tileset;
	numTilesX = 0;
	numTilesY = 0;
}