#include "Wave.h"
#include <GL/freeglut.h>
#define _USE_MATH_DEFINES
#include <math.h>

Wave::Wave(float width, float length, float amplitude) : amplitude(amplitude),
patch(Vector3(0, 0, 0), Vector3(width / 3, amplitude * 4 / 5, 0), Vector3(2 * width / 3, amplitude * 4 / 5, 0), Vector3(width, 0, 0),
Vector3(0, 0, length / 3), Vector3(width / 3, amplitude * 4 / 5, length / 3), Vector3(2 * width / 3, amplitude * 4 / 5, length / 3), Vector3(width, 0, length / 3),
Vector3(0, 0, 2*length / 3), Vector3(width / 3, amplitude * 4 / 5, 2*length / 3), Vector3(2 * width / 3, amplitude * 4 / 5, 2*length / 3), Vector3(width, 0, 2*length / 3),
Vector3(0, 0, length), Vector3(width / 3, amplitude * 4 / 5, length), Vector3(2 * width / 3, amplitude * 4 / 5, length), Vector3(width, 0, length))
{
}

void Wave::draw(void)
{
	draw(10);
}

void Wave::draw(size_t subdivs)
{
	size_t sub_inc = subdivs + 1;
	// Points
	GLfloat * ps = new GLfloat[sub_inc * sub_inc * 3];
	// Normals
	GLfloat * ns = new GLfloat[sub_inc * sub_inc * 3];
	/* Indices; we'll draw quads in order to save some space.
	 * Using triangles instead of quads to leave our options open.
	 */
	GLuint * indices = new GLuint[subdivs * subdivs * 6];
	// No textures will be used... probably...

	// Calculate points
	for (unsigned int v = 0; v <= subdivs; ++v)
	{
		for (unsigned int u = 0; u <= subdivs; ++u)
		{
			Vector3 p = patch.interpolate(u, v);
			ps[v * subdivs * 3 + u * 3] = p[0];
			ps[v * subdivs * 3 + u * 3 + 1] = p[1];
			ps[v * subdivs * 3 + u * 3 + 2] = p[2];
		}
	}

	// Calculate indices
	for (unsigned int row = 0; row < subdivs; ++row)
	{
		for (unsigned int col = 0; col < subdivs; ++col)
		{
			indices[row * subdivs * 6 + col * 6] = (row + 1) * sub_inc + col;
			indices[row * subdivs * 6 + col * 6 + 1] = (row + 1) * sub_inc + col + 1;
			indices[row * subdivs * 6 + col * 6 + 2] = row * sub_inc + col + 1;
			indices[row * subdivs * 6 + col * 6 + 3] = (row + 1) * sub_inc + col + 1;
			indices[row * subdivs * 6 + col * 6 + 4] = row * sub_inc + col + 1;
			indices[row * subdivs * 6 + col * 6 + 5] = row * sub_inc + col;
		}
	}
}

void Wave::tick(void)
{
	float mod_tick = this->tick_count / (M_PI*24);
	++this->tick_count;
}
