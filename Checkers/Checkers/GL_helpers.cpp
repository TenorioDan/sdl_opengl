#include "GL_helpers.h"

void checkGL_Error(GLenum error, bool& success)
{
	if (error != GL_NO_ERROR)
	{
		printf("GL Error! %s\n", gluErrorString(error));
	}
}