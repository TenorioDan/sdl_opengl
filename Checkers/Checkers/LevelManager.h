#pragma once

#include <string>
#include <vector>
#include "GameObject.h"

class LevelManager
{
public:
	LevelManager();
	~LevelManager();
	
	bool loadMedia();
	void createLevel(std::string tilePath, std::string colliderPath);
	void spawnGameObjects();

private:
	
};