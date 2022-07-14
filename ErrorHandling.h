#ifndef ERRORHANDLING_H
#define ERRORHANDLING_H

#include <string>

class ErrorHandling
{
public:
	static void testGlError(int);
	static void testGlError(std::string);
	static void testGlError(std::string, int);
	static void testGlError(void);
};

#endif // ERRORHANDLING_H