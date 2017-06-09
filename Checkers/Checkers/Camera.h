#pragma once
#include "GL_helpers.h"
#include "GameObject.h"

struct Camera
{
	// TODO: Keep here if we decided to change the way cameras work
	GLfloat positionX = 0;
	GLfloat positionY = 0;
	GLfloat offsetX = 0;
	GLfloat offsetY = 0;
	GLfloat minX = 0;
	GLfloat minY = 0;
	GameObject* objectToFollow;

	void render()
	{
		glLoadIdentity();
		
		GLfloat objectPositionX = objectToFollow->PositionX();
		GLfloat objectPositionY = objectToFollow->PositionY();

		if (objectPositionX > minX + offsetX)
		{
			glTranslatef(-objectToFollow->PositionX() + offsetX, 0.f, 0.f);
		}
		
		if (objectPositionY > minY + offsetY)
		{
			glTranslatef(0.f, -objectToFollow->PositionY() + offsetY, 0.f);
		}

	}
};