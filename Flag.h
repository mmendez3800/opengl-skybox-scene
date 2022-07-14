#ifndef FLAG_H
#define FLAG_H

#include "BezierPatch.h"
#include "Texture.h"

class Flag
{
private:
	BezierPatch flag_surface;
	Texture tex;
	unsigned int frag_shader;
	unsigned int vert_shader;
	unsigned int prog;

public:
	Flag(BezierPatch, Texture);
	void draw(void);
};

#endif // FLAG_H
