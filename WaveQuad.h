#ifndef WAVEQUAD_H
#define WAVEQUAD_H

#include "Shader.h"
#include "VertexArrayObject.h"
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Texture.h"
#include "Camera.h"

class WaveQuad
{
private:
	VertexArrayObject vao;
	GLuint reflect_texture;
	GLuint refract_texture;
	static Shader * shader;
	static Texture * dudv;
	static int instances;
	void checkInitShader(void);
	void checkDeleteShader(void);
	GLfloat dudvOffset;

public:
	WaveQuad(Vector3, Vector3, Vector3, Vector3, GLuint, GLuint);
	~WaveQuad(void);

	void draw(Camera &, Matrix4 &, Vector4 &);
	void tick(void);
};

#endif // WAVEQUAD_H
