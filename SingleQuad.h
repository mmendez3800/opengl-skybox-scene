#ifndef SINGLEQUAD_H
#define SINGLEQUAD_H

#include "Vector3.h"
#include "Shader.h"
#include "Camera.h"

// For rendering a single quad
class SingleQuad
{
private:
	GLuint vbo_verts;
	GLuint vbo_inds;
	GLuint vao;
	Shader shader;

public:
	SingleQuad(Vector3 a, Vector3 b, Vector3 c, Vector3 d);
	~SingleQuad(void);

	void draw(Camera &, Matrix4 &);
};

#endif // SINGLEQUAD_H
