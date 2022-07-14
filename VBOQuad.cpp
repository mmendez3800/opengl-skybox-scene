#include "VBOQuad.h"
#include "ErrorHandling.h"
#include <cstdio>

Shader * VBOQuad::shader;
int VBOQuad::instances;

void VBOQuad::testInitShader(void)
{
	if (shader == nullptr)
	{
		shader = new Shader("quad.vert", "quad.frag", true);
		ErrorHandling::testGlError(__FILE__, __LINE__);
		shader->printLog("post-compile");
		ErrorHandling::testGlError(__FILE__, __LINE__);
	}
}

VBOQuad::VBOQuad(Vector3 a, Vector3 b, Vector3 c, Vector3 d)
{
	testInitShader();
	++instances;
	static GLuint indices[] = { 0, 1, 2, 0, 2, 3 };
	GLfloat vertices[12] = { a[0], a[1], a[2], b[0], b[1], b[2], c[0], c[1], c[2], d[0], d[1], d[2] };
	vao.bindDataToAttribute(shader->getAttribLocation("vertex"), 3, vertices, sizeof(vertices));
	vao.bindIndexArrayData(indices, sizeof(indices));
	ent = new ShadedEnt(&vao, shader);
}

VBOQuad::~VBOQuad(void)
{
	--instances;
	if (instances == 0)
	{
		delete shader;
		shader = nullptr;
	}
}

void VBOQuad::draw(Matrix4 & cam_inv, Matrix4 & dp, Vector4 & clip)
{
	ent->draw(cam_inv, dp, clip);
}
