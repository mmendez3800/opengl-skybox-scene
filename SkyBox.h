#ifndef SKYBOX_H
#define SKYBOX_H

#include "Shader.h"
#include "VertexArrayObject.h"
#include "Texture.h"
#include "Matrix4.h"
#include "Vector4.h"

class SkyBox
{
private:
	Matrix4 rotation;
	Shader * shader;
	VertexArrayObject vao_front;
	VertexArrayObject vao_back;
	VertexArrayObject vao_left;
	VertexArrayObject vao_right;
	VertexArrayObject vao_top;
	VertexArrayObject vao_bottom;

	void initializeBack(void);
	void initializeFront(void);
	void initializeLeft(void);
	void initializeRight(void);
	void initializeTop(void);
	void initializeBottom(void);

public:
	Texture front, back, left, right, top, base;

	SkyBox();
	SkyBox(Texture, Texture, Texture, Texture, Texture, Texture);
	void draw(Matrix4&, Matrix4&, Vector4&);
	void tick(void);
};

#endif // SKYBOX_H
