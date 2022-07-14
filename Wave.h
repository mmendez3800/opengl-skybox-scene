#ifndef WAVE_H
#define WAVE_H

#include "BezierPatch.h"

class Wave
{
private:
	// Shader information shared by all waves
	static char* frag_shader;
	// Shader information shared by all waves
	static char* vert_shader;
	// Handle to the fragment shader shared by all waves
	static unsigned int frag_shader_handle;
	// Handle to the vertex shader shared by all waves
	static unsigned int vert_shader_handle;
	// Handle to the shader program shared by all waves
	static unsigned int shader_prog_handle;
	float amplitude;
	int tick_count;
	BezierPatch patch;

public:
	// Width, length, amplitude
	Wave(float, float, float);
	// Draw the wave
	void draw(void);
	// Draw the wave with a given number of subdivisions along each axis
	void draw(size_t);
	// Causes the wave to transform itself a little bit
	void tick(void);
};

#endif // WAVE_H
