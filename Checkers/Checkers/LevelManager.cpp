#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include "LevelManager.h"
#include "MediaManager.h"

LevelManager::LevelManager()
{ 
	transitionOffset = 40;
}

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
	clearEnemies();
}

void LevelManager::buildLevel(std::string path)
{
	clearTiles();
	clearEnemies();
	transitions.clear();
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
			tileCount = std::stoi(*++it);
			tileset = new Tile*[tileCount];
		}
		else if (*it == "COLLIDERS")
		{
			readState = Colliders;
		}
		else if (*it == "ENEMIES")
		{
			readState = Enemies;
		}
		else if (*it == "TRANSITIONS")
		{
			readState = Transitions;
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
					createTile(it);
					break;
				case Colliders:
					createCollider(it);
					break;
				case Enemies:
					spawnEnemy(it);
				case Transitions:
					createTransition(it);
			}
		}
	}
}

void LevelManager::createTransition(std::vector<std::string>::iterator it)
{
	Transition t;
	std::string name = *it++;
	t.transitionLevelName = *it++;
	t.transitionTileName = *it++;
	int xVal = std::stoi(*it++);
	int yVal = std::stoi(*it);
	t.collider.minX = xVal - tileWidth / 2;
	t.collider.minY = yVal - tileHeight / 2;
	t.collider.maxX = xVal + tileWidth / 2;
	t.collider.maxY = yVal + tileHeight / 2;
	transitions.insert(std::pair<std::string, Transition>(name, t));
}

// Build the tile based and add it to the list of current tiles
void LevelManager::createTile(std::vector<std::string>::iterator it)
{
	Tile* t = new Tile();
	
	t->positionX = std::stoi(*it++);
	t->positionY = std::stoi(*it++);
	t->spriteIndex = std::stoi(*it++) - 1;
	tileset[currentTile++] = t;
}


void LevelManager::createCollider(std::vector<std::string>::iterator it)
{
	Collider* c = new Collider();
	c->minX = std::stoi(*it++) - (tileWidth / 2.f);
	c->minY = std::stoi(*it++) - (tileWidth / 2.f);
	c->maxX = std::stoi(*it++) - (tileWidth / 2.f);
	c->maxY = std::stoi(*it++) - (tileWidth / 2.f);
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


void LevelManager::transitionToLevel(std::string transitionLevelName, std::string transitionTileName, Collider::CollisionDirection direction, GLfloat previousY)
{
	buildLevel("Levels/" + transitionLevelName + ".lvl");
	Transition t = transitions.at(transitionTileName);
	GLfloat newYPosition = t.collider.minY + (player.PositionY() - previousY);
	
	if (direction == Collider::LEFT)
	{
		printf("LEFT\n");
		player.setPosition(t.collider.maxX + transitionOffset, newYPosition);
	}
	else
	{
		printf("RIGHT\n");
		player.setPosition(t.collider.minX - transitionOffset, newYPosition);
	}

}

void LevelManager::updateEnemies(int time)
{
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

// Check if the player has collided with any of the transitions, if so transition them to that new 
// level/room
void LevelManager::checkTransitions()
{
	std::string transitionLevelName = "";
	std::string transitionTileName = "";
	Collider::CollisionDirection direction = Collider::CollisionDirection::NO_COLLISION;
	bool beginTransition = false;
	int previousY = 0;

	for (auto it = transitions.begin(); it != transitions.end(); it++)
	{
		Collider::CollisionDirection currentDirection = player.getCollider().collision(it->second.collider);
		if (currentDirection != Collider::NO_COLLISION)
		{
			beginTransition = true;
			direction = currentDirection;
			previousY = it->second.collider.minY;
			transitionLevelName = it->second.transitionLevelName;
			transitionTileName = it->second.transitionTileName;
		}
	}

	if (beginTransition)
	{
		transitionToLevel(transitionLevelName, transitionTileName, direction, previousY);
	}
}

// Update loop. Check collisions and stuff
void LevelManager::update(int time)
{
	player.update(time);
	updateEnemies(time);
}


// Render all the tiles in the tileset
void LevelManager::renderTileset()
{
	for (int i = 0; i < tileCount; ++i)
	{
		// Move to the spot to render and then move back to render the next texture 
		// in the correct spot 
		Tile t = *tileset[i];
		glTranslatef(t.positionY, t.positionX, 0.f);
		tileSheet->renderSprite(t.spriteIndex);
		glTranslatef(-t.positionY, -t.positionX, 0.f);
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

// Clear the tileset to give room for loading different levels
void LevelManager::clearTiles()
{
	delete[] tileset;
	tileCount = 0;
	currentTile = 0;

	for (auto it = platforms.begin(); it != platforms.end(); ++it)
	{
		delete (*it);
	}

	platforms.clear();
}

void LevelManager::clearEnemies()
{
	for (auto it = enemies.begin(); it != enemies.end(); ++it)
	{
		delete(*it);
	}

	enemies.clear();
}