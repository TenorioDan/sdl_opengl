#pragma once
#include "Texture.h"
#include <vector>

class SpriteSheet : public Texture
{
public:
	SpriteSheet();
	~SpriteSheet();

	// Adds clipping rectangle to clip array
	int addClipSprite(LFRect& newClip);

	// Returns clipping rectangle at given point
	LFRect getClip(int index);

	// Generates VBO and IBO to render sprites with
	bool generateDataBuffer();

	// Deallocates member VBO, IBO, and clip array
	void freeSheet();

	// Frees sprite sheet and base Texture
	void freeTexture();

	// Renders sprite at given index
	void renderSprite(int index);

protected:
	// Sprite clips
	std::vector<LFRect> mClips;

	// VBO data
	GLuint mVertexDataBuffer;
	GLuint* mIndexBuffers;
	
};