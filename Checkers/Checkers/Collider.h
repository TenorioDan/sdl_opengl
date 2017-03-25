#pragma once

#include "LFRect.h"

class Collider
{
public:
	Collider();
	void setBounds(GLuint maxX, GLuint maxY, GLuint minX, GLuint minY);

	GLfloat MaxX();
	GLfloat MaxY();
	GLfloat MinX();
	GLfloat MinY();
	bool collision(Collider c);

private:
	GLfloat maxX;
	GLfloat maxY;
	GLfloat minX;
	GLfloat minY;
};