#include "MediaManager.h"

MediaManager::MediaManager() { }
MediaManager* MediaManager::instance;

MediaManager* MediaManager::getInstance()
{
	if (instance == NULL)
	{
		instance = new MediaManager();
	}

	return instance;
}

MediaManager::~MediaManager()
{
	clearMedia();
	delete instance;
}

SpriteSheet* MediaManager::getSpriteSheet(std::string filename)
{
	return spriteSheets.at(filename);
}

// Long ass function for loading the media that will be needed in a particular section of the game.
// Takes a string that will determine what files should be loaded. 
// I think when this happens a loading screen may need to be displayed
bool MediaManager::loadMedia(std::string timePeriodName)
{
	clearMedia();
	bool success = true;
	// Create the sprite sheets
	SpriteSheet* mainCharacterSpriteSheet = new SpriteSheet();
	SpriteSheet* basicProjectileSpriteSheet = new SpriteSheet();
	SpriteSheet* starMonsterSpriteSheet = new SpriteSheet();
	SpriteSheet* tileSheet = new SpriteSheet();

	// Load the textures
	success = mainCharacterSpriteSheet->loadTextureFromFileWithColorKey("shovel_knight_original.png", 255, 255, 255);
	success = basicProjectileSpriteSheet->loadTextureFromFileWithColorKey("blaster-1.png", 255, 0, 255);
	success = starMonsterSpriteSheet->loadTextureFromFileWithColorKey("star_enemy.png", 255, 0, 255);
	success = tileSheet->loadTextureFromFileWithColorKey("tileset_platforms.png", 255, 0, 255);

	if (!success)
	{
		printf("Unable to load a spritesheet\n");
		return success;
	}

	// Create the sprite sheet animations
	// TODO: Fix magic numbers and maybe create an object that contains all int/float values for objects in the game
	mainCharacterSpriteSheet->createAnimations(85.f, 87.f, 5.f, 266.f, 10.f, 4);
	basicProjectileSpriteSheet->createAnimations(20.f, 20.f, 0.f, 0.f, 0.f, 1);
	starMonsterSpriteSheet->createAnimations(50.f, 50.f, 0.f, 0.f, 0.f, 1);

	// Manually clip the tile sheet since these technically aren't animations
	// set clips
	LFRect clip = { 0.f, 0.f, 64.f, 64.f };

	// Top left
	clip.x = 0.f;
	clip.y = 0.f;
	tileSheet->addClipSprite(clip);

	// Top right
	clip.x = 64.f + tileSpriteOffeset;
	clip.y = 0.f;
	tileSheet->addClipSprite(clip);

	// Bottom left
	clip.x = 128.f + (tileSpriteOffeset * 2);
	clip.y = 0.f;
	tileSheet->addClipSprite(clip);

	// TODO: Fix sprite sheet tile index allocations
	for (int i = 0; i < 9; ++i)
	{
		clip.x = (i * tileWidth) + (tileSpriteOffeset * i);
		clip.y = 74.f;
		tileSheet->addClipSprite(clip);
	}

	// Generate data buffers
	success = mainCharacterSpriteSheet->generateDataBuffer();
	success = basicProjectileSpriteSheet->generateDataBuffer();
	success = starMonsterSpriteSheet->generateDataBuffer();
	success = tileSheet->generateDataBuffer();

	if (!success)
	{
		printf("Unable to clip a spritesheet\n");
		return success;
	}
	
	// Insert the sprite sheets into the map with their associated keys
	spriteSheets.insert(std::pair<std::string, SpriteSheet*>("MAIN_CHARACTER", mainCharacterSpriteSheet));
	spriteSheets.insert(std::pair<std::string, SpriteSheet*>("BASIC_PROJECTILE", basicProjectileSpriteSheet));
	spriteSheets.insert(std::pair<std::string, SpriteSheet*>("STAR_MONSTER", starMonsterSpriteSheet));
	spriteSheets.insert(std::pair<std::string, SpriteSheet*>("TILE_SHEET", tileSheet));
}

void MediaManager::clearMedia()
{
	for each (auto s in spriteSheets)
	{
		delete s.second;
	}

	spriteSheets.clear();
}