#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
//#include <afx.h>
#include <iterator>
#include <Windows.h>
#include "TileManager.h"

TileManager::TileManager() 
{

}


bool TileManager::loadMedia()
{
	


	return true;
}

// Called when creating the tilset of a new level.
void TileManager::createTileset(std::string path)
{
	
}



// Create the colliders from the provided text file
void TileManager::createColliders(std::string path)
{
	// Read the platforms file and create colliders based on input
	std::ifstream colliderFile(path.c_str());
	std::string line;

	while (std::getline(colliderFile, line))
	{
		std::istringstream buffer(line);
		std::istream_iterator<std::string> beg(buffer), end;
		std::vector<std::string> values(beg, end);
		auto it = values.begin();

		Collider* c = new Collider();
		c->minX = std::stoi(*it++) - (tileWidth / 2.f);
		c->minY = std::stoi(*it++) - (tileWidth / 2.f);
		c->maxX = std::stoi(*it++) - (tileWidth / 2.f);
		c->maxY = std::stoi(*it) - (tileWidth / 2.f);
		platforms.push_back(c);
	}
}


// Return the colliders int his level. Typically used for collision detection
std::vector<Collider*>* TileManager::getPlatforms()
{
	return &platforms;
}



