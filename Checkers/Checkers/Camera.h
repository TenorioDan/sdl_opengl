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
	GameObject* objectToFollow;

	void render()
	{
		glLoadIdentity();
		glTranslatef(-objectToFollow->PositionX() + offsetX, -objectToFollow->PositionY() + offsetY, 0.f);
	}
};