#pragma once
#include "GameObject.h"
#include "SpriteSheet.h"

class Platform : public GameObject
{
public:
	Platform();
	void update(int time);
	void render();
	bool loadMedia();
private:
	Texture platformTexture;
};