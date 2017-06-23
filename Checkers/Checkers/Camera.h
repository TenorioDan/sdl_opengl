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
	GLfloat minBoundaryX = 0;
	GLfloat minBoundaryY = 0;
	GameObject* objectToFollow;

	void render()
	{
		glLoadIdentity();
		
		GLfloat objectPositionX = objectToFollow->PositionX();
		GLfloat objectPositionY = objectToFollow->PositionY();

		glTranslatef((-objectPositionX + offsetX) * (objectPositionX > (offsetX)), 
					 (-objectPositionY + offsetY) * (objectPositionY > (offsetY)),
					 0.f);
		
		// glTranslatef(0.f, -objectToFollow->PositionY() + offsetY, 0.f);

	}
};