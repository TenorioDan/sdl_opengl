#pragma once

#include <string>
#include <map>
#include "SpriteSheet.h"

class MediaManager
{
public:
	static MediaManager* getInstance();
	~MediaManager();
	
	void clearMedia();
	bool loadMedia(std::string timePeriodName);
	SpriteSheet* getSpriteSheet(std::string filename);

private:
	static MediaManager* instance;
	MediaManager();

	const GLfloat tileWidth = 64.f;
	const GLfloat tileHeight = 64.f;
	GLfloat tileSpriteOffeset = 10;

	std::map<const std::string, SpriteSheet*> spriteSheets;
	GLfloat mainCharacterWidth;
	GLfloat mainCharacterHeight;
};