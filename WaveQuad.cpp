#include <GL/GLee.h>
#include "WaveQuad.h"
#include "ErrorHandling.h"

#define DUDV_OFFSET_INC 0.0001f;

int WaveQuad::instances(0);
Shader * WaveQuad::shader(nullptr);
Texture * WaveQuad::dudv(nullptr); 

WaveQuad::WaveQuad(Vector3 a, Vector3 b, Vector3 c, Vector3 d, GLuint reflect, GLuint refract) : reflect_texture(reflect), refract_texture(refract)
{
	GLfloat vertices[] = { a[0], a[1], a[2], b[0], b[1], b[2], c[0], c[1], c[2], d[0], d[1], d[2] };
	GLfloat texcoords[] = { 0, 0, 0, 1, 1, 1, 1, 0 };
	GLuint indices[] = { 0, 1, 2, 0, 2, 3 };
	checkInitShader();

	vao.bind();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	vao.bindDataToAttribute(shader->getAttribLocation("vertex"), 3, vertices, sizeof(vertices));
	ErrorHandling::testGlError(__FILE__, __LINE__);
	vao.bindDataToAttribute(shader->getAttribLocation("tex_coord"), 2, texcoords, sizeof(texcoords));
	ErrorHandling::testGlError(__FILE__, __LINE__);
	vao.bindIndexArrayData(indices, sizeof(indices));
	ErrorHandling::testGlError(__FILE__, __LINE__);
	vao.unbind();
}

WaveQuad::~WaveQuad(void)
{
	checkDeleteShader();
}

void WaveQuad::checkInitShader(void)
{
	if (instances == 0)
	{
		shader = new Shader("water.vert", "water.frag", true);
		dudv = new Texture("waterDUDV.ppm");
	}
	++instances;
}

void WaveQuad::checkDeleteShader(void)
{
	if (--instances == 0)
	{
		delete shader;
		delete dudv;
	}
}

void WaveQuad::draw(Camera & cam, Matrix4 & P, Vector4 & clip)
{
	// Standard bindings
	shader->bind();
	vao.bind();

	// Prepare our textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflect_texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refract_texture);
	glActiveTexture(GL_TEXTURE2);
	dudv->bind();
	glUniform1i(shader->getUniformLocation("reflect"), 0);
	glUniform1i(shader->getUniformLocation("refract"), 1);
	glUniform1i(shader->getUniformLocation("dudv"), 2);

	// Special water things
	glUniform1f(shader->getUniformLocation("dudvOffset"), dudvOffset);
	glUniform3fv(shader->getUniformLocation("cam_pos"), 1, cam.e.ptr());

	// Standard uniforms
	glUniformMatrix4fv(shader->getUniformLocation("cam_inv"), 1, GL_FALSE, cam.getInverseMatrix().ptr());
	glUniformMatrix4fv(shader->getUniformLocation("proj"), 1, GL_FALSE, P.ptr());
	glUniform4fv(shader->getUniformLocation("clip"), 1, clip.ptr());

	// Draw the thing
	vao.render();

	// Clean up
	// We already hvae texture2 selected
	dudv->unbind();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	vao.unbind();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	shader->unbind();
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

void WaveQuad::tick(void)
{
	dudvOffset += DUDV_OFFSET_INC;
	if (dudvOffset >= 1.0f)
	{
		dudvOffset -= 1.0f;
	}
}
