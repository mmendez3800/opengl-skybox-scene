#include "ErrorHandling.h"
#include <GL/freeglut.h>
#include <cstdio>

void ErrorHandling::testGlError(void)
{
	int error = glGetError();
	if (error)
	{
		printf("%s\n", gluErrorString(error));
	}
}

void ErrorHandling::testGlError(int line)
{
	int error = glGetError();
	if (error)
	{
		printf("%d: %s\n", line, gluErrorString(error));
	}
}

void ErrorHandling::testGlError(std::string prefix)
{
	int error = glGetError();
	if (error)
	{
		printf("%s: %s\n", prefix, gluErrorString(error));
	}
}

void ErrorHandling::testGlError(std::string prefix, int line)
{
	int error = glGetError();
	if (error)
	{
		printf("%s: %d: %s\n", prefix.c_str(), line, gluErrorString(error));
	}
}
