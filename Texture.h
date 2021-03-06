#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>

class Texture
{
protected:
	const char* filename;

	static unsigned char* loadPPM(const char*, int&, int&);

public:
	unsigned int id;

	static Texture* emptyTexture;

	Texture(void);
	Texture(const char* filename);
	virtual ~Texture(void);

	void bind(void);
	void unbind(void);
};

#endif
