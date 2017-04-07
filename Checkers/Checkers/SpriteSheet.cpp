#include "SpriteSheet.h"
#include "LVertexData2D.h"

SpriteSheet::SpriteSheet()
{
	// Initialize vertex buffer data
	mVertexDataBuffer = NULL;
	mIndexBuffers = NULL;
}

SpriteSheet::~SpriteSheet()
{
	// clear sprite sheet data
	freeSheet();
}

int SpriteSheet::addClipSprite(LFRect& newClip)
{
	// Add clip and return index
	mClips.push_back(newClip);
	return mClips.size() - 1;
}

LFRect SpriteSheet::getClip(int index)
{
	return mClips[index];
}

bool SpriteSheet::generateDataBuffer()
{
	// If there is a texture loaded and clips to make vertex data from
	if (getTextureID() != 0 && mClips.size() > 0)
	{
		// Allocate vertex buffer data
		int totalSprites = mClips.size();
		LVertexData2D* vertexData = new LVertexData2D[totalSprites * 4];
		mIndexBuffers = new GLuint[totalSprites];

		// Allocate vertex data buffer name
		glGenBuffers(1, &mVertexDataBuffer);

		// Allocate index buffers names
		glGenBuffers(totalSprites, mIndexBuffers);

		// Go through clips
		GLfloat tW = textureWidth();
		GLfloat tH = textureHeight();
		GLuint spriteIndices[4] = { 0, 0, 0, 0 };

		for (int i = 0; i < totalSprites; ++i)
		{
			// Initialize indices
			spriteIndices[0] = i * 4 + 0;
			spriteIndices[1] = i * 4 + 1;
			spriteIndices[2] = i * 4 + 2;
			spriteIndices[3] = i * 4 + 3;

			// Top left
			vertexData[spriteIndices[0]].position.x = -mClips[i].w / 2.f;
			vertexData[spriteIndices[0]].position.y = -mClips[i].h / 2.f;

			vertexData[spriteIndices[0]].texCoord.s = mClips[i].x / tW;
			vertexData[spriteIndices[0]].texCoord.t = mClips[i].y / tH;

			// Top Right
			vertexData[spriteIndices[1]].position.x =  mClips[i].w / 2.f;
			vertexData[spriteIndices[1]].position.y = -mClips[i].h / 2.f;

			vertexData[spriteIndices[1]].texCoord.s = (mClips[i].x + mClips[i].w) / tW;
			vertexData[spriteIndices[1]].texCoord.t =  mClips[i].y / tH;

			// Bottom right
			vertexData[spriteIndices[2]].position.x =  mClips[i].w / 2.f;
			vertexData[spriteIndices[2]].position.y =  mClips[i].h / 2.f;

			vertexData[spriteIndices[2]].texCoord.s = (mClips[i].x + mClips[i].w) / tW;
			vertexData[spriteIndices[2]].texCoord.t = (mClips[i].y + mClips[i].h)/ tH;

			// Bottom left
			vertexData[spriteIndices[3]].position.x = -mClips[i].w / 2.f;
			vertexData[spriteIndices[3]].position.y =  mClips[i].h / 2.f;

			vertexData[spriteIndices[3]].texCoord.s =  mClips[i].x / tW;
			vertexData[spriteIndices[3]].texCoord.t = (mClips[i].y + mClips[i].h) / tH;

			// Bind sprite index buffer data
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffers[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), spriteIndices, GL_STATIC_DRAW);
		}

		// Bind vertex data
		glBindBuffer(GL_ARRAY_BUFFER, mVertexDataBuffer);
		glBufferData(GL_ARRAY_BUFFER, totalSprites * 4 * sizeof(LVertexData2D), vertexData, GL_STATIC_DRAW);

		// Deallocate vertex data
		delete[] vertexData;
	}
	else
	{
		if (getTextureID() == 0)
		{
			printf("No texture to render with!\n");
		}
		if (mClips.size() <= 0)
		{
			printf("No clips to generate vertex data from!\n");
		}
		
		return false;
	}

	return true;
}

void SpriteSheet::freeSheet()
{
	// Clear vertex buffer
	if (mVertexDataBuffer != NULL)
	{
		glDeleteBuffers(1, &mVertexDataBuffer);
	}

	// Clear index buffers
	if (mIndexBuffers != NULL)
	{
		glDeleteBuffers(mClips.size(), mIndexBuffers);
		delete[] mIndexBuffers;
		mIndexBuffers = NULL;
	}

	mClips.clear();
}

void SpriteSheet::freeTexture()
{
	// Get rid of sprite sheet data
	freeSheet();

	// Free texture
	Texture::freeTexture();
}

void SpriteSheet::renderSprite(int index)
{
	// Sprite sheed data exists
	if (mVertexDataBuffer != NULL)
	{
		// Set texture
		glBindTexture(GL_TEXTURE_2D, getTextureID());

		// Enable vertex and texture coordinate arrays
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
		// Bind vertex data
		glBindBuffer(GL_ARRAY_BUFFER, mVertexDataBuffer);

		// Set texture coordinate data
		glTexCoordPointer(2, GL_FLOAT, sizeof(LVertexData2D), (GLvoid*) offsetof(LVertexData2D, texCoord));
		glVertexPointer(2, GL_FLOAT, sizeof(LVertexData2D), (GLvoid*) offsetof(LVertexData2D, position));

		// Draw quad using vertex data and index data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffers[index]);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, NULL);

		// Disable vertex and texture coordinate arrays
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}

// Create the character animations based on the spritesheets provided
void SpriteSheet::createAnimations(GLfloat spriteWidth, GLfloat spriteHeight, GLfloat spriteOffset,
	GLfloat spriteStartPositionX, GLfloat spriteStartPositionY, int animationCount)
{
	// TODO: Create character spritesheet and define locations
	// set clips
	LFRect clip = { 0.f, 0.f, spriteWidth, spriteHeight };

	// Add running right sprites
	for (int i = 0; i < animationCount; ++i)
	{
		clip.x = spriteStartPositionX + (spriteWidth * i) + (spriteOffset * i);
		clip.y = spriteStartPositionY;
		addClipSprite(clip);
	}

	for (int i = 0; i < animationCount; ++i)
	{
		clip.x = imageWidth() - (spriteStartPositionX + (spriteWidth * (i + 1)) + (spriteOffset * i));
		clip.y = spriteStartPositionY;
		addClipSprite(clip);
	}
}
