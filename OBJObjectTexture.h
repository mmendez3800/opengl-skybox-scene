#ifndef OBJOBJECTTEXTURE_H
#define OBJOBJECTTEXTURE_H

#include "Shader.h"
#include <iostream>
#include <vector>
#include <string>
#include "Vector3.h"
#include "Vector2.h"
#include "Drawable.h"
#include "Texture.h"

struct Face
{
	int vertexIndices[3];
	int normalIndices[3];
	int colorIndices[3];
	int textureIndices[3];
	Material *mat;

	//~Face() {delete mat;}
};

class OBJObjectTexture : public Drawable
{

protected:

	static Shader * shader;
	static void checkInitShader(void);
	static void checkDeleteShader(void);
	static int instances;
	//Storage vectors
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector3> colors;
	std::vector<Vector2> textures;
	std::vector<Face> faces;

	int num_texcoords;

	//std::vector<Material> materials;
	std::string textName;
	Texture texture;

	std::vector<Vector3> vertexNormals;
	std::vector<int> indices;
	std::vector<int> normIndices;
	std::vector<int> textIndices;

	//Helper functions
	std::vector<std::string>& split(const std::string&, char, std::vector<std::string>&);
	std::vector<std::string> split(const std::string&, char);

	//Parse
	void parse(std::string&);
	void parseMTL(std::string&);

public:
	std::vector<Material> materials;
	OBJObjectTexture(std::string, int);
	virtual ~OBJObjectTexture(void);

	virtual void draw(Matrix4&, Matrix4&, Matrix4&, Vector4&);
	virtual void update(UpdateData&);

	std::vector<Vector3> * getVertices(void);
	std::vector<int> *  getIndices(void);
	std::vector<Vector3> * getNormals(void);
	std::vector<int> * getNormIndices(void);

	void centerObject(void);
	void scaleObject(float);
	void resetObject(void);

	Matrix4 starting;
};

#endif