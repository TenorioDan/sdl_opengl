#include "Texture.h"
#include <SDL_image.h>
#include <IL/il.h>;
#include <IL/ilu.h>
#include "LVertexData2D.h"


#pragma region Texture Loading

Texture::Texture()
{
	// Initialize texture ID
	mTextureID = 0;
	mPixels = NULL;

	// Initialize image dimensions
	mImageWidth = 0;
	mImageHeight = 0;

	// Initialize texture dimensions
	mTextureWidth = 0;
	mTextureHeight = 0;

	// Initialize VBO
	mVBOID = 0;
	mIBOID = 0;
}

void Texture::initVBO()
{
	// If texture is loaded and VBO does not already exist
	if (mTextureID != 0 && mVBOID == 0)
	{
		// Vertex data
		LVertexData2D vData[4];
		GLuint iData[4];

		// SEt rendering indices
		iData[0] = 0;
		iData[1] = 1;
		iData[2] = 2;
		iData[3] = 3;

		// Create VBO
		glGenBuffers(1, &mVBOID);
		glBindBuffer(GL_ARRAY_BUFFER, mVBOID);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(LVertexData2D), vData, GL_DYNAMIC_DRAW);

		// Create IBO
		glGenBuffers(1, &mIBOID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBOID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), iData, GL_DYNAMIC_DRAW);
	}
}

Texture::~Texture()
{
	// Dealocate texture
	freeTexture();

	// Free VBO and IBO if needed
	freeVBO();
}

void Texture::freeTexture()
{
	// Delete texture
	if (mTextureID != 0)
	{
		glDeleteTextures(1, &mTextureID);
		mTextureID = 0;
	}

	// Delete pixels
	if (mPixels != NULL)
	{
		delete[] mPixels;
		mPixels = NULL;
	}

	mImageWidth = 0;
	mImageHeight = 0;
	mTextureWidth = 0;
	mTextureHeight = 0;
}

void Texture::freeVBO()
{
	// Free VBO and IBO
	if (mVBOID != 0)
	{
		glDeleteBuffers(1, &mVBOID);
		glDeleteBuffers(1, &mIBOID);
	}
}

bool Texture::loadPixelsFromFile(std::string path)
{
	// Deallocate texture data
	freeTexture();

	// Texture loading success
	bool pixelsLoaded = false;

	// Generate and set current image ID
	ILuint imgID = 0;
	ilGenImages(1, &imgID);
	ilBindImage(imgID);
	
	// Load image
	ILboolean success = ilLoadImage(path.c_str());

	// Image loaded successfully
	if (success == IL_TRUE)
	{
		// Convert image to RGBA
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (success = IL_TRUE)
		{
			// Initialize dimensions
			GLuint imgWidth = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
			GLuint imgHeight = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);

			// Calculate required texture dimensions
			GLuint texWidth = powerOfTwo(imgWidth);
			GLuint texHeight = powerOfTwo(imgHeight);

			// If Texture is the wrong size
			if (imgWidth != texWidth || imgHeight != texHeight)
			{
				// Place image at upper left
				iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);

				// Resize image
				iluEnlargeCanvas((int)texWidth, (int)texHeight, 1);
			}

			// Allocates memory for texture data
			GLuint size = texWidth * texHeight;
			mPixels = new GLuint[size];

			// Get image dimensions
			mImageWidth = imgWidth;
			mImageHeight = imgHeight;
			mTextureWidth = texWidth;
			mTextureHeight = texHeight;

			// Copy pixels
			memcpy(mPixels, ilGetData(), size * 4);
			pixelsLoaded = true;
		}

		ilDeleteImages(1, &imgID);
	}

	if (!pixelsLoaded)
	{
		printf("Unable to load %s\n", path.c_str());
	}

	return pixelsLoaded;
}


bool Texture::loadTextureFromFile(std::string path)
{
	bool textureLoaded = false;

	// Generate and set current image ID
	ILuint imgID = 0;
	ilGenImages(1, &imgID);
	ilBindImage(imgID);


	//load iamge
	ILboolean success = ilLoadImage(path.c_str());

	// Image loaded successfully
	if (success == IL_TRUE)
	{
		// Convert image to RGBA
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		if (success == IL_TRUE)
		{
			// Initialize dimensions
			GLuint imgWidth = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
			GLuint imgHeight = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);

			// Calculate required texture dimensions
			GLuint texWidth = powerOfTwo(imgWidth);
			GLuint texHeight = powerOfTwo(imgHeight);

			if (imgWidth != texWidth || imgHeight != texHeight)
			{
				// Place image at upper left
				iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);

				// Resize image
				iluEnlargeCanvas((int)texWidth, (int)texHeight, 1);
			}

			// Create texture from file pixels
			textureLoaded = loadTextureFromPixels32((GLuint*)ilGetData(), imgWidth, imgHeight, texWidth, texHeight);
		}

		// Delete file from memory
		ilDeleteImages(1, &imgID);
	}

	// Report error
	if (!textureLoaded)
	{
		printf("Unable to laod %s\n", path.c_str());
	}

	return textureLoaded;
}

bool Texture::loadTextureFromFileWithColorKey(std::string path, GLubyte r, GLubyte g, GLubyte b, GLubyte a)
{
	// Load pixels
	if (!loadPixelsFromFile(path))
	{
		return false;
	}

	// Go through pixels
	GLuint size = mTextureWidth * mTextureHeight;
	for (int i = 0; i < size; ++i)
	{
		// Get pixel colors
		GLubyte* colors = (GLubyte*)&mPixels[i];

		// Color matches
		if (colors[0] == r && colors[1] == g && colors[2] == b && (0 == a || colors[3] == a))
		{
			// Make transparent
			colors[0] = 255;
			colors[1] = 255;
			colors[2] = 255;
			colors[3] = 000;
		}
	}

	return loadTextureFromPixels32();
}

// New code for padding non-power-of-two textures
bool Texture::loadTextureFromPixels32(GLuint* pixels, GLuint imgWidth, GLuint imgHeight, GLuint texWidth, GLuint texHeight)
{
	// Free texture if it exists
	freeTexture();
	
	// Get image dimensions
	mImageWidth = imgWidth;
	mImageHeight = imgHeight;
	mTextureWidth = texWidth;
	mTextureHeight = texHeight;

	// Generate texture ID
	glGenTextures(1, &mTextureID);

	// Bind texture ID
	glBindTexture(GL_TEXTURE_2D, mTextureID);

	// Generate Texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, DEFAULT_TEXTURE_WRAP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, DEFAULT_TEXTURE_WRAP);

	// Ubind Texture
	glBindTexture(GL_TEXTURE_2D, NULL);

	//Check for error
	bool success = true;
	checkGL_Error(glGetError(), success);

	initVBO();

	return success;
	
}

bool Texture::loadTextureFromPixels32()
{
	// loading flag
	bool success = true;

	// There is loaded pixels
	if (mTextureID == 0 && mPixels != NULL)
	{
		// Generate texture ID
		glGenTextures(1, &mTextureID);

		// Bind texture ID
		glBindTexture(GL_TEXTURE_2D, mTextureID);

		// Generate texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mTextureWidth, mTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mPixels);

		// Set texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		// Unbind texture
		glBindTexture(GL_TEXTURE_2D, NULL);

		// Check for error
		GLenum error = glGetError();
		
		if (error != GL_NO_ERROR)
		{
			printf("Error loading texture %p pixels! %s\n", mPixels, gluErrorString(error));
			success = false;
		}
		else
		{
			// Release pixels
			delete[] mPixels;
			mPixels = NULL;
		}
	}
	else
	{
		printf("Cannot load texture from current pixels! ");
		
		// Texture already exists
		if (mTextureID != 0)
		{
			printf("A texture is already loaded!\n");
		}
		// No pixel loaded
		else if (mPixels == NULL)
		{
			printf("No pixels to create texture from!\n");
		}

		success = false;
	}

	return success;
}

void Texture::render(GLfloat x, GLfloat y, LFRect* clip)
{
	//If the texture exists
	if (mTextureID != 0)
	{
		//Texture coordinates
		GLfloat texTop = 0.f;
		GLfloat texBottom = (GLfloat)mImageHeight / (GLfloat)mTextureHeight;
		GLfloat texLeft = 0.f;
		GLfloat texRight = (GLfloat)mImageWidth / (GLfloat)mTextureWidth;

		//Vertex coordinates
		GLfloat quadWidth = mImageWidth;
		GLfloat quadHeight = mImageHeight;

		//Handle clipping
		if (clip != NULL)
		{
			//Texture coordinates
			texLeft = clip->x / mTextureWidth;
			texRight = (clip->x + clip->w) / mTextureWidth;
			texTop = clip->y / mTextureHeight;
			texBottom = (clip->y + clip->h) / mTextureHeight;

			//Vertex coordinates
			quadWidth = clip->w;
			quadHeight = clip->h;
		}

		//Move to rendering point
		glTranslatef(x, y, 0.f);

		// Set vertex data
		LVertexData2D vData[4];

		// Texture coordinates
		vData[0].texCoord.s = texLeft;  vData[0].texCoord.t = texTop;
		vData[1].texCoord.s = texRight; vData[1].texCoord.t = texTop;
		vData[2].texCoord.s = texRight; vData[2].texCoord.t = texBottom;
		vData[3].texCoord.s = texLeft;  vData[3].texCoord.t = texBottom;

		// Vertex position
		vData[0].position.x = 0.f;       vData[0].position.y = 0.f;
		vData[1].position.x = quadWidth; vData[1].position.y = 0.f;
		vData[2].position.x = quadWidth; vData[2].position.y = quadHeight;
		vData[3].position.x = 0.f;       vData[3].position.y = quadHeight;

		//Render textured quad
		glBegin(GL_QUADS);
			glTexCoord2f(texLeft,  texTop);    glVertex2f(0.f,       0.f);
			glTexCoord2f(texRight, texTop);    glVertex2f(quadWidth, 0.f);
			glTexCoord2f(texRight, texBottom); glVertex2f(quadWidth, quadHeight);
			glTexCoord2f(texLeft,  texBottom); glVertex2f(0.f,       quadHeight);
		glEnd();

		//Set texture ID
		glBindTexture(GL_TEXTURE_2D, mTextureID);

		// Enable vertex and texure coordinate arryas
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		// Bind vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, mVBOID);

		// Update vertex buffer data
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(LVertexData2D), vData);

		// Set texture coordinate data
		glTexCoordPointer(2, GL_FLOAT, sizeof(LVertexData2D), (GLvoid*)offsetof(LVertexData2D, texCoord));

		// Set vertex data
		glVertexPointer(2, GL_FLOAT, sizeof(LVertexData2D), (GLvoid*)offsetof(LVertexData2D, position));

		// Draw quad using vertex data and index data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBOID);
		glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, NULL);

		// Disable vertex and texture coordinate arrays
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
	}
}
bool Texture::lock()
{
	// If texture is not locked and a texture exists
	if (mPixels == NULL && mTextureID != 0)
	{
		// Allocates memory for texture data
		GLuint size = mTextureWidth * mTextureHeight;
		mPixels = new GLuint[size];

		// Set current texture
		glBindTexture(GL_TEXTURE_2D, mTextureID);

		// Get pixels
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, mPixels);

		// Unbind texture
		glBindTexture(GL_TEXTURE_2D, NULL);

		return true;
	}

	return false;
}

bool Texture::unlock()
{
	// If texture is locked and a texture exists
	if (mPixels != NULL && mTextureID != 0)
	{
		// Set current texture
		glBindTexture(GL_TEXTURE_2D, mTextureID);

		// Update texture
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mTextureWidth, mTextureHeight, GL_RGBA, GL_UNSIGNED_BYTE, mPixels);

		// Delete pixels
		delete[] mPixels;
		mPixels = NULL;

		// Unbind texture
		glBindTexture(GL_TEXTURE_2D, NULL);

		return true;
	}

	return false;
}

GLuint Texture::powerOfTwo(GLuint num)
{
	if (num != 0)
	{
		num--;
		num |= (num >> 1); // Or first two bits
		num |= (num >> 2); // Or next 2 bits
		num |= (num >> 4); // Or next 4 bits
		num |= (num >> 8); // Or next 4 bits
		num |= (num >> 16); // or next 16 bits
		num++;
	}

	return num;
}
#pragma endregion

#pragma region Getter/Setters

GLuint* Texture::getPixelData32()
{
	return mPixels;
}

GLuint Texture::getPixel32(GLuint x, GLuint y)
{
	return mPixels[y * mTextureWidth + x];
}

void Texture::setPixel32(GLuint x, GLuint y, GLuint pixel)
{
	mPixels[y * mTextureWidth + x] = pixel;
}


GLuint Texture::getTextureID()
{
	return mTextureID;
}

GLuint Texture::textureWidth()
{
	return mTextureWidth;
}

GLuint Texture::textureHeight()
{
	return mTextureHeight;
}

GLuint Texture::imageWidth()
{
	return mImageWidth;
}

GLuint Texture::imageHeight()
{
	return mImageHeight;
}

#pragma endregion
