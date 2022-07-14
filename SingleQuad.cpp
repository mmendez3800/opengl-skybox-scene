#include <GL/GLee.h>
#include "SingleQuad.h"
#include <cstdio>
#include "ErrorHandling.h"

SingleQuad::SingleQuad(Vector3 a, Vector3 b, Vector3 c, Vector3 d) : shader("quad.vert", "quad.frag", true)
{
	shader.printLog();
	GLfloat vertices[12];
	vertices[0] = a[0];
	vertices[1] = a[1];
	vertices[2] = a[2];
	vertices[3] = b[0];
	vertices[4] = b[1];
	vertices[5] = b[2];
	vertices[6] = c[0];
	vertices[7] = c[1];
	vertices[8] = c[2];
	vertices[9] = d[0];
	vertices[10] = d[1];
	vertices[11] = d[2];
	GLuint indices[4] = { 0, 1, 2, 3 };

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo_verts);
	glGenBuffers(1, &vbo_inds);

	//printf("Attrib location: %d\n", shader.getAttribLocation("vertex"));

	glBindBuffer(GL_ARRAY_BUFFER, vbo_verts);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(shader.getAttribLocation("vertex"));
	glBindAttribLocation(shader.getPid(), shader.getAttribLocation("vertex"), "vertex");
	glVertexAttribPointer(shader.getAttribLocation("vertex"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_inds);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// these lines below are used to verify that the element array buffer binding is tied to the vao
	//GLint elebuf = 0;
	//glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elebuf);
	//printf("Element buffer: %d\n", elebuf);
	glBindVertexArray(0);
	//glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &elebuf);
	//printf("Element buffer: %d\n", elebuf);

	ErrorHandling::testGlError(__FILE__, __LINE__);
}

SingleQuad::~SingleQuad(void)
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo_inds);
	glDeleteBuffers(1, &vbo_verts);
}

void SingleQuad::draw(Camera & camera, Matrix4 & dp)
{
	glDisable(GL_LIGHTING);
	static GLubyte indices[] = {0, 1, 2, 3};
	shader.bind();
	glBindBuffer(GL_ARRAY_BUFFER, vbo_verts);
	ErrorHandling::testGlError(__LINE__);
	/*
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, indices);
	*/
	glEnableClientState(GL_VERTEX_ARRAY);
	ErrorHandling::testGlError(__LINE__);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	ErrorHandling::testGlError(__LINE__);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(.5f, 0.0f, 0.0f);
	ErrorHandling::testGlError(__LINE__);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, indices);
	glDisable(GL_COLOR_MATERIAL);
	ErrorHandling::testGlError(__LINE__);
	glDisableClientState(GL_VERTEX_ARRAY);
	ErrorHandling::testGlError(__LINE__);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	shader.unbind();
	glEnable(GL_LIGHTING);
#ifdef NEVER_DO_THIS
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glBindVertexArray(vao);
	shader.bind();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glUniformMatrix4fv(shader.getUniformLocation("cam_inv"), 1, GL_FALSE, camera.getInverseMatrix().ptr());
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glUniformMatrix4fv(shader.getUniformLocation("dp"), 1, GL_FALSE, dp.ptr());
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glEnableVertexAttribArray(shader.getAttribLocation("vertex"));
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glDrawElements(GL_TRIANGLE_STRIP, 3, GL_UNSIGNED_BYTE, 0);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glBindVertexArray(0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	shader.unbind();
	ErrorHandling::testGlError(__FILE__, __LINE__);
#endif
}
