#include "Collider.h"

Collider::Collider()
{
	maxX = 0;
	maxY = 0;
	minX = 0;
	minY = 0;
}


void Collider::setBounds(GLfloat maxX, GLfloat maxY, GLfloat minX, GLfloat minY)
{
	this->maxX = maxX;
	this->maxY = maxY;
	this->minX = minX;
	this->minY = minY;
}


bool Collider::collision(Collider c)
{
	return  (minX <= c.MaxX() && maxX >= c.MinX()) &&
			(minY <= c.MaxY() && maxY >= c.MinY());
}

GLfloat Collider::MaxX()
{
	return maxX;
}

GLfloat Collider::MaxY()
{
	return maxY;
}

GLfloat Collider::MinX()
{
	return minX;
}

GLfloat Collider::MinY()
{
	return minY;
}