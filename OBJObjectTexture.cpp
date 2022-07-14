#include <GL/GLee.h>
#include "OBJObjectTexture.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES

#include "Window.h"
#include "math.h"
#include <sstream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include "Vector4.h"
#include "Globals.h"
#include "ErrorHandling.h"

#define deleteVector(__type__, __vect__) do {\
                                   std::vector<__type__>::iterator iter; \
                                   std::vector<__type__>::iterator end; \
                                   iter = __vect__->begin();\
                                   end = __vect__->end();\
								                                      while(iter != end) delete (*(iter++));\
                                   delete __vect__;\
							                                  } while(false)

Material * current;
Shader * OBJObjectTexture::shader(nullptr);
int OBJObjectTexture::instances(0);

OBJObjectTexture::OBJObjectTexture(std::string filename, int flag) : Drawable()
{
	parse(filename);

	if (flag == 2)
		scaleObject(14.72f);

	else if (flag == 3)
		scaleObject(18.44f);


	else if (flag == 4)
		scaleObject(14.26f);

	centerObject();

	starting = toWorld;
	checkInitShader();
}

OBJObjectTexture::~OBJObjectTexture()
{
	std::vector<Vector3>().swap(vertices);
	std::vector<Vector3>().swap(normals);
	std::vector<Vector3>().swap(colors);
	std::vector<Vector2>().swap(textures);
	std::vector<Face>().swap(faces);
	std::vector<Material>().swap(materials);
	checkDeleteShader();
}

void OBJObjectTexture::checkInitShader(void)
{
	if (instances == 0)
	{
		shader = new Shader("obj_shader.vert", "obj_shader.frag", true);
	}
	++instances;
}

void OBJObjectTexture::checkDeleteShader(void)
{
	if (--instances == 0)
	{
		delete shader;
		shader = nullptr;
	}
}

void OBJObjectTexture::draw(Matrix4& cam_inv, Matrix4& proj, Matrix4& transform, Vector4& clip)
{
	shader->bind();
	GLuint vertex_location = shader->getAttribLocation("vertex");
	GLuint tex_location = shader->getAttribLocation("tex_pos");
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glUniformMatrix4fv(shader->getUniformLocation("cam_inv"), 1, GL_FALSE, cam_inv.ptr());
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glUniformMatrix4fv(shader->getUniformLocation("proj"), 1, GL_FALSE, proj.ptr());
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glUniformMatrix4fv(shader->getUniformLocation("transform"), 1, GL_FALSE, transform.ptr());
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glUniform4fv(shader->getUniformLocation("clip"), 1, clip.ptr());
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glUniform1i(shader->getUniformLocation("tex"), 0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	//material.apply();

	glMatrixMode(GL_MODELVIEW);
	ErrorHandling::testGlError(__FILE__, __LINE__);

	glPushMatrix();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glMultMatrixf(toWorld.ptr());
	ErrorHandling::testGlError(__FILE__, __LINE__);

	//Loop through the faces
	//For each face:
	//  Look up the vertices, normals (if they exist), and texcoords (if they exist)
	//  Draw them as triplets:
	ErrorHandling::testGlError(__FILE__, __LINE__);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	for (unsigned int i = 0; i < faces.size(); i++) {
		Face face = faces.at(i);
		/*if (i%100 == 0)
		std::cout << face.mat.getTextureName() << std::endl;*/

		if (textName.empty()) {
			texture = face.mat->picture;
			textName.assign(face.mat->getTextureName());
			face.mat->apply();
			texture.bind();
			//std::cout << "empty called" << std::endl;
		}

		else if (textName.compare(face.mat->getTextureName()) != 0) {
			texture.unbind();
			texture = face.mat->picture;
			textName.assign(face.mat->getTextureName());
			face.mat->apply();
			texture.bind();
			//std::cout << "compare called" << std::endl;
		}
		
		//printf("num tex coords %d\n", num_texcoords);

		glBegin(GL_TRIANGLES);
		for (int j = 0; j < 3; j++) {
			//printf("at %d of %d", face.textureIndices[j] - 1, num_texcoords);
			//glColor3f(1.0, 1.0, 1.0);
			//glNormal3fv((normals.at(face.normalIndices[j] - 1)).ptr());

			glTexCoord2fv((textures.at(face.textureIndices[j] - 1)).ptr());
			glVertexAttrib2fv(tex_location, (textures.at(face.textureIndices[j] - 1)).ptr());
			//glVertexAttrib3fv(vertex_location, (vertices.at(face.vertexIndices[j] - 1)).ptr());
			//glVertex3i(0, 0, 0);
			glVertex3fv((vertices.at(face.vertexIndices[j] - 1)).ptr());
		}
		glEnd();
	}

	ErrorHandling::testGlError(__FILE__, __LINE__);
	texture.unbind();
	ErrorHandling::testGlError(__FILE__, __LINE__);

	ErrorHandling::testGlError(__FILE__, __LINE__);
	glPopMatrix();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	shader->unbind();
}

void OBJObjectTexture::update(UpdateData& data)
{
}

void OBJObjectTexture::parse(std::string& filename)
{
	std::ifstream infile(filename);
	std::string line;
	std::vector<std::string> tokens, f_1, f_2, f_3;

	int lineNum = 0;


	std::cout << "Starting parse..." << std::endl;

	//While all your lines are belong to us
	while (std::getline(infile, line))
	{
		//Progress
		if (++lineNum % 10000 == 0)
			std::cout << "At line " << lineNum << std::endl;

		//Split a line into tokens by delimiting it on spaces
		//"Er Mah Gerd" becomes ["Er", "Mah", "Gerd"]
		tokens.clear();
		tokens = split(line, ' ', tokens);

		if (tokens.empty()){
			continue;
		}

		if (tokens.at(0).compare("mtllib") == 0)
		{
			parseMTL(tokens.at(1));
		}

		//If first token is a v then it gots to be a vertex
		else if (tokens.at(0).compare("v") == 0)
		{
			//Parse the vertex line
			float x = std::stof(tokens.at(1));
			float y = std::stof(tokens.at(2));
			float z = std::stof(tokens.at(3));

			vertices.push_back(Vector3(x, y, z));

			if (tokens.size() > 4) {
				float r = std::stof(tokens.at(4));
				float g = std::stof(tokens.at(5));
				float b = std::stof(tokens.at(6));

				colors.push_back(Vector3(r, g, b));
			}
		}
		else if (tokens.at(0).compare("vt") == 0)
		{
			float xt = std::stof(tokens.at(1));
			float yt = std::stof(tokens.at(2));

			textures.push_back(Vector2(xt, -yt));
			++num_texcoords;
		}
		else if (tokens.at(0).compare("vn") == 0)
		{
			//Parse the normal line
			float xn = std::stof(tokens.at(1));
			float yn = std::stof(tokens.at(2));
			float zn = std::stof(tokens.at(3));

			Vector3 norm = Vector3(xn, yn, zn);
			vertexNormals.push_back(norm);

			norm = norm.normalize();
			normals.push_back(norm);
		}
		else if (tokens.at(0).compare("usemtl") == 0)
		{
			for (unsigned int i = 0; i < materials.size(); i++) {
				if (tokens.at(1).compare(materials[i].getMaterialName()) == 0){
					//std::cout << "Name is " << materials[i].getMaterialName() << std::endl;
					current = &materials[i];
					//std::cout << "New name is " << current.getMaterialName() << std::endl;
				}
			}
		}
		else if (tokens.at(0).compare("f") == 0)
		{
			Face face;

			//Parse the face line
			f_1.clear();
			f_2.clear();
			f_3.clear();

			f_1 = split(tokens.at(1), '/', f_1);
			f_2 = split(tokens.at(2), '/', f_2);
			f_3 = split(tokens.at(3), '/', f_3);

			face.vertexIndices[0] = std::stof(f_1.at(0));
			face.colorIndices[0] = std::stof(f_1.at(0));
			face.textureIndices[0] = std::stof(f_1.at(1));
			face.normalIndices[0] = std::stof(f_1.at(2));

			face.vertexIndices[1] = std::stof(f_2.at(0));
			face.colorIndices[1] = std::stof(f_1.at(0));
			face.textureIndices[1] = std::stof(f_2.at(1));
			face.normalIndices[1] = std::stof(f_2.at(2));

			face.vertexIndices[2] = std::stof(f_3.at(0));
			face.colorIndices[2] = std::stof(f_1.at(0));
			face.textureIndices[2] = std::stof(f_3.at(1));
			face.normalIndices[2] = std::stof(f_3.at(2));

			face.mat = current;

			faces.push_back(face);

			indices.push_back(face.vertexIndices[0] - 1);
			indices.push_back(face.vertexIndices[1] - 1);
			indices.push_back(face.vertexIndices[2] - 1);

			textIndices.push_back(face.textureIndices[0] - 1);
			textIndices.push_back(face.textureIndices[1] - 1);
			textIndices.push_back(face.textureIndices[2] - 1);

			normIndices.push_back(face.normalIndices[0] - 1);
			normIndices.push_back(face.normalIndices[1] - 1);
			normIndices.push_back(face.normalIndices[2] - 1);
		}
		else if (tokens.at(0).compare("#") == 0)
		{
			//See the WavemaxZ Object format specification for details
			continue;
		}
	}

	std::cout << "Done parsing." << std::endl;
	//std::cout << "Finished at line " << lineNum << std::endl;
}

void OBJObjectTexture::parseMTL(std::string& filename)
{
	std::ifstream infile(filename);
	std::string line, name;
	std::vector<std::string> tokens, f_1, f_2, f_3;
	Material *temp = NULL;

	int lineNum = 0;

	std::cout << "Sub parse..." << std::endl;
	//While all your lines are belong to us
	while (std::getline(infile, line))
	{
		//Split a line into tokens by delimiting it on spaces
		//"Er Mah Gerd" becomes ["Er", "Mah", "Gerd"]
		tokens.clear();
		tokens = split(line, ' ', tokens);

		if (tokens.empty()){
			continue;
		}

		if (tokens.at(0).compare("newmtl") == 0)
		{
			//name.assign(temp->getMaterialName());
			if (temp != NULL){
				materials.push_back(*temp);
				temp = NULL;
				//std::cout << "newmtl is called" << std::endl;
			}

			temp = new Material();
			temp->setMaterialName(tokens.at(1));
		}
		else if (tokens.at(0).compare("ka") == 0)
		{
			float x = std::stof(tokens.at(1));
			float y = std::stof(tokens.at(2));
			float z = std::stof(tokens.at(3));

			temp->setAmbient(x, y, z);
			//std::cout << "ka is called" << std::endl;
		}
		else if (tokens.at(0).compare("kd") == 0)
		{
			float x = std::stof(tokens.at(1));
			float y = std::stof(tokens.at(2));
			float z = std::stof(tokens.at(3));

			temp->setDiffuse(x, y, z);
			//std::cout << "ka is called" << std::endl;
		}
		else if (tokens.at(0).compare("ks") == 0)
		{
			float x = std::stof(tokens.at(1));
			float y = std::stof(tokens.at(2));
			float z = std::stof(tokens.at(3));

			temp->setSpecular(x, y, z);
			//std::cout << "ka is called" << std::endl;
		}
		else if (tokens.at(0).compare("ke") == 0)
		{
			float x = std::stof(tokens.at(1));
			float y = std::stof(tokens.at(2));
			float z = std::stof(tokens.at(3));

			temp->setEmission(x, y, z);
			//std::cout << "ka is called" << std::endl;
		}
		else if (tokens.at(0).compare("map_kd") == 0)
		{
			temp->setTextureName(tokens.at(1));
			//std::cout << tokens.at(1) << std::endl;
		}
		else if (tokens.at(0).compare("#") == 0)
		{
			//See the WavemaxZ Object format specification for details
			continue;
		}
	}
	materials.push_back(*temp);

	std::cout << "Done with sub parse." << std::endl;
}

std::vector<Vector3>* OBJObjectTexture::getVertices()
{
	return &vertices;
}

std::vector<int>* OBJObjectTexture::getIndices()
{
	return &indices;
}

std::vector<Vector3>* OBJObjectTexture::getNormals()
{
	return &vertexNormals;
}

std::vector<int>* OBJObjectTexture::getNormIndices()
{
	return &normIndices;
}

void OBJObjectTexture::centerObject()
{
	std::vector<Vector3>* vertices = getVertices();

	Vector3 startVec = toWorld * vertices->at(0);

	float minX = startVec[0];
	float maxX = startVec[0];
	float maxY = startVec[1];
	float minY = startVec[1];
	float maxZ = startVec[2];
	float minZ = startVec[2];

	std::vector<Vector3>::iterator itr = vertices->begin();

	while (itr != vertices->end())
	{
		Vector3 vec = toWorld * (*itr++);

		minX = min(vec[0], minX);
		maxX = max(vec[0], maxX);
		maxY = max(vec[1], maxY);
		minY = min(vec[1], minY);
		maxZ = max(vec[2], maxZ);
		minZ = min(vec[2], minZ);
	}

	float xCenter = (maxX + minX) / 2;
	float yCenter = (maxY + minY) / 2;
	float zCenter = (maxZ + minZ) / 2;

	Matrix4 translate;
	translate.makeTranslate(-xCenter, -yCenter, -zCenter);

	toWorld = translate * toWorld;
}

void OBJObjectTexture::scaleObject(float factor)
{
	std::vector<Vector3>* vertices = getVertices();

	Vector3 startVec = toWorld * vertices->at(0);

	float minX = startVec[0];
	float maxX = startVec[0];
	float maxY = startVec[1];
	float minY = startVec[1];

	std::vector<Vector3>::iterator itr = vertices->begin();

	while (itr != vertices->end())
	{
		Vector3 vec = toWorld * (*itr++);

		minX = min(vec[0], minX);
		maxX = max(vec[0], maxX);
		maxY = max(vec[1], maxY);
		minY = min(vec[1], minY);
	}

	float length = (maxX - minX) / 2;

	Matrix4 scale;
	scale.makeScale(factor * tan(30.0f * M_PI / 180.0f) / length);

	toWorld = toWorld * scale;
}

void OBJObjectTexture::resetObject()
{
	toWorld = starting;
}

std::vector<std::string>& OBJObjectTexture::split(const std::string &s, char delim, std::vector<std::string> &elems)
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> OBJObjectTexture::split(const std::string &s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}
