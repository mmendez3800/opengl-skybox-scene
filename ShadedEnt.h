#ifndef SHADEDENT_H
#define SHADEDENT_H

#include "Shader.h"
#include "VertexArrayObject.h"
#include "Matrix4.h"
#include "Vector4.h"

class ShadedEnt
{
private:
	VertexArrayObject * vao;
	Shader * shader;

public:
	ShadedEnt(VertexArrayObject *, Shader *);

	void draw(Matrix4 &, Matrix4&, Vector4 &);
};

#endif // SHADEDENT_H
