#ifndef VBOQUAD_H
#define VBOQUAD_H

#include "Shader.h"
#include "VertexArrayObject.h"
#include "ShadedEnt.h"
#include "Vector3.h"
#include "Matrix4.h"
#include "Vector4.h"

class VBOQuad
{
private:
	VertexArrayObject vao;
	static Shader * shader;
	static int instances;
	ShadedEnt * ent;

	void testInitShader(void);

public:
	VBOQuad(Vector3, Vector3, Vector3, Vector3);
	~VBOQuad(void);

	void draw(Matrix4 &, Matrix4 &, Vector4 &);
};

#endif VBOQUAD_H
