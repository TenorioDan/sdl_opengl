#pragma once

#include "LFRect.h"

class Collider
{
public:
	Collider();
	void setBounds(GLfloat maxX, GLfloat maxY, GLfloat minX, GLfloat minY);

	GLfloat MaxX();
	GLfloat MaxY();
	GLfloat MinX();
	GLfloat MinY();

private:
	GLfloat maxX;
	GLfloat maxY;
	GLfloat minX;
	GLfloat minY;
	bool collision(Collider c);
};