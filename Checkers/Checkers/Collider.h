#pragma once

#include "LFRect.h"

struct Collider
{
public:
	enum CollisionDirection
	{
		ABOVE,
		BELOW,
		LEFT,
		RIGHT,
		COLLISION,
		NO_COLLISION
	};

	GLfloat maxX = 0;
	GLfloat maxY = 0;
	GLfloat minX = 0;
	GLfloat minY = 0;

	GLfloat prevMaxX = 0;
	GLfloat prevMaxY = 0;
	GLfloat prevMinX = 0;
	GLfloat prevMinY = 0;


	CollisionDirection collision(Collider c)
	{
		if ((minX <= c.maxX && maxX >= c.minX) &&
			(minY <= c.maxY && maxY >= c.minY))
		{
			if (prevMaxX < c.minX && maxX >= c.minX)
			{
				return LEFT;
			}	
			else if (prevMinX >= c.maxX && minX < c.maxX)
			{
				return RIGHT;
			}	
			else if (prevMaxY < c.minY && maxY >= c.minY)
			{
				return ABOVE;
			}	
			else if (prevMinY >= c.maxY && minY < c.maxY)
			{
				return BELOW;
			}
			
			return COLLISION;
		}

		//printf("none\n");
		return NO_COLLISION;
	}
};