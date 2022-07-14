#include "SkyBox.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/freeglut.h>
#include "ErrorHandling.h"
#include <cstdio>

#define SKYBOX_SIZE 400.0f

SkyBox::SkyBox()
{
	rotation.identity();
}

SkyBox::SkyBox(Texture front, Texture back, Texture top, Texture base, Texture left, Texture right) : front(front), back(back), base(base), top(top), left(left), right(right), vao_front(), vao_back()
{
	rotation.identity();
	shader = new Shader("skybox.vert", "skybox.frag", true);
	initializeBack();
	initializeFront();
	initializeRight();
	initializeTop();
	initializeLeft();
	initializeBottom();
}

void SkyBox::initializeTop(void)
{
	static GLfloat vertices[] = {
		-SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
		SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
		SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
		-SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE
	};
	static GLfloat texcoords[] = {
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};
	static GLuint indices[] = { 0, 1, 2, 0, 2, 3 };

	vao_top.bindDataToAttribute(shader->getAttribLocation("vertex"), 3, vertices, sizeof(vertices));
	vao_top.bindDataToAttribute(shader->getAttribLocation("tex_coord"), 2, texcoords, sizeof(texcoords));
	vao_top.bindIndexArrayData(indices, sizeof(indices));
}

void SkyBox::initializeBack(void)
{
	static GLfloat vertices[] = {
		-SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
		SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
		SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
		-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
	};
	static GLfloat texcoords[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};
	static GLuint indices[] =
	{
		0, 1, 2, 0, 2, 3
	};

	vao_back.bindDataToAttribute(shader->getAttribLocation("vertex"), 3, vertices, sizeof(vertices));
	vao_back.bindDataToAttribute(shader->getAttribLocation("tex_coord"), 2, texcoords, sizeof(texcoords));
	vao_back.bindIndexArrayData(indices, sizeof(indices));
}

void SkyBox::initializeFront(void)
{
	static GLfloat vertices[] = {
		-SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
		SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
		SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
		-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
	};
	static GLfloat texcoords[] = {
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
	};
	static GLuint indices[] = { 0, 1, 2, 0, 2, 3 };

	vao_front.bindDataToAttribute(shader->getAttribLocation("vertex"), 3, vertices, sizeof(vertices));
	vao_front.bindDataToAttribute(shader->getAttribLocation("tex_coord"), 2, texcoords, sizeof(texcoords));
	vao_front.bindIndexArrayData(indices, sizeof(indices));
}

void SkyBox::initializeLeft(void)
{
	static GLfloat vertices[] = {
		-SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
		-SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
		-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
		-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
	};
	static GLfloat texcoords[] = {
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f
	};
	static GLuint indices[] = {0, 1, 2, 0, 2, 3};

	vao_left.bindDataToAttribute(shader->getAttribLocation("vertex"), 3, vertices, sizeof(vertices));
	vao_left.bindDataToAttribute(shader->getAttribLocation("tex_coord"), 2, texcoords, sizeof(texcoords));
	vao_left.bindIndexArrayData(indices, sizeof(indices));
}

void SkyBox::initializeRight(void)
{
	static GLfloat vertices[] = {
		SKYBOX_SIZE, SKYBOX_SIZE, -SKYBOX_SIZE,
		SKYBOX_SIZE, SKYBOX_SIZE, SKYBOX_SIZE,
		SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
		SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE
	};
	static GLfloat texcoords[] = {
		0, 0,
		1, 0,
		1, 1,
		0, 1
	};
	static GLuint indices[] = {0, 1, 2, 0, 2, 3};

	vao_right.bindDataToAttribute(shader->getAttribLocation("vertex"), 3, vertices, sizeof(vertices));
	vao_right.bindDataToAttribute(shader->getAttribLocation("tex_coord"), 2, texcoords, sizeof(texcoords));
	vao_right.bindIndexArrayData(indices, sizeof(indices));
}

void SkyBox::initializeBottom(void)
{
	static GLfloat vertices[] = {
		-SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
		SKYBOX_SIZE, -SKYBOX_SIZE, -SKYBOX_SIZE,
		SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE,
		-SKYBOX_SIZE, -SKYBOX_SIZE, SKYBOX_SIZE
	};
	static GLfloat texcoords[] = {
		1, 1,
		0, 1,
		0, 0,
		1, 0
	};
	static GLuint indices[] = { 0, 1, 2, 0, 2, 3 };

	vao_bottom.bindDataToAttribute(shader->getAttribLocation("vertex"), 3, vertices, sizeof(vertices));
	vao_bottom.bindDataToAttribute(shader->getAttribLocation("tex_coord"), 2, texcoords, sizeof(texcoords));
	vao_bottom.bindIndexArrayData(indices, sizeof(indices));
}

void SkyBox::draw(Matrix4 & cam_inv, Matrix4 & P, Vector4 & clip)
{
	ErrorHandling::testGlError(__FILE__, __LINE__);
	//glDisable(GL_LIGHTING);
	shader->bind();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glUniformMatrix4fv(shader->getUniformLocation("cam_inv"), 1, GL_FALSE, cam_inv.ptr());
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glUniformMatrix4fv(shader->getUniformLocation("proj"), 1, GL_FALSE, P.ptr());
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glUniform4fv(shader->getUniformLocation("clip"), 1, clip.ptr());
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glUniform1i(shader->getUniformLocation("tex"), 0);
	glUniformMatrix4fv(shader->getUniformLocation("rotation"), 1, GL_FALSE, rotation.ptr());
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glPushMatrix();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glMultMatrixf(rotation.ptr());
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glColor3f(1, 1, 1);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	back.bind();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	vao_back.render();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	back.unbind();
	ErrorHandling::testGlError(__FILE__, __LINE__);

	front.bind();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	vao_front.render();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	front.unbind();
	ErrorHandling::testGlError(__FILE__, __LINE__);

	top.bind();
	vao_top.render();
	top.unbind();

	right.bind();
	vao_right.render();
	right.unbind();

	left.bind();
	vao_left.render();
	left.unbind();

	base.bind();
	vao_bottom.render();
	base.unbind();
	glPopMatrix();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	//glEnable(GL_LIGHTING);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	shader->unbind();
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

void SkyBox::tick(void)
{
	Matrix4 new_rot;
	new_rot.makeRotateY(M_PI / 10240);
	rotation = new_rot * rotation;
}
