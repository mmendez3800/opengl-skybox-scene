#include "House.h"
#include <GL/glut.h>

House::House(void)
{
	this->toWorld.identity();
	initializeColors();
}

void House::draw(DrawData& data)
{
	// This data structure defines a simple house

	int nVerts = 42;    // your vertex array needs to have this many entries

	// These are the x,y,z coordinates of the vertices of the triangles
	static GLfloat vertices[] = {
		-4, -4, 4, 4, -4, 4, 4, 4, 4, -4, 4, 4,     // front face
		-4, -4, -4, -4, -4, 4, -4, 4, 4, -4, 4, -4, // left face
		4, -4, -4, -4, -4, -4, -4, 4, -4, 4, 4, -4,  // back face
		4, -4, 4, 4, -4, -4, 4, 4, -4, 4, 4, 4,     // right face
		4, 4, 4, 4, 4, -4, -4, 4, -4, -4, 4, 4,     // top face
		-4, -4, 4, -4, -4, -4, 4, -4, -4, 4, -4, 4, // bottom face

		-20, -4, 20, 20, -4, 20, 20, -4, -20, -20, -4, -20, // grass
		-4, 4, 4, 4, 4, 4, 0, 8, 4,                       // front attic wall
		4, 4, 4, 4, 4, -4, 0, 8, -4, 0, 8, 4,               // left slope
		-4, 4, 4, 0, 8, 4, 0, 8, -4, -4, 4, -4,             // right slope
		4, 4, -4, -4, 4, -4, 0, 8, -4 };                   // rear attic wall

	// These are the RGB colors corresponding to the vertices, in the same order
	static GLfloat colors[] = {
		1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,  // front is red
		0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,  // left is green
		1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,  // back is red
		0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,  // right is green
		0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,  // top is blue
		0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1,  // bottom is blue

		0, 0.5, 0, 0, 0.5, 0, 0, 0.5, 0, 0, 0.5, 0, // grass is dark green
		0, 0, 1, 0, 0, 1, 0, 0, 1,                // front attic wall is blue
		1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0,         // left slope is green
		0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0,         // right slope is red
		0, 0, 1, 0, 0, 1, 0, 0, 1, };              // rear attic wall is red

	// The index data stores the connectivity of the triangles; 
	// index 0 refers to the first triangle defined above
	static GLuint indices[] = {
		0, 2, 3, 0, 1, 2,      // front face
		4, 6, 7, 4, 5, 6,      // left face
		8, 10, 11, 8, 9, 10,     // back face
		12, 14, 15, 12, 13, 14,   // right face
		16, 18, 19, 16, 17, 18,   // top face
		20, 22, 23, 20, 21, 22,   // bottom face

		24, 26, 27, 24, 25, 26,   // grass
		28, 29, 30,             // front attic wall
		31, 33, 34, 31, 32, 33,   // left slope
		35, 37, 38, 35, 36, 37,   // right slope
		39, 40, 41 };            // rear attic wall	glMatrixMode(GL_MODELVIEW);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, colors);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, indices);
}

std::vector<Vector3*>* House::getVertices()
{
	static Vector3* vertices[] = {
		new Vector3(-4, -4, 4), new Vector3(4, -4, 4), new Vector3(4, 4, 4), new Vector3(-4, 4, 4),    // front face
		new Vector3(-4, -4, -4), new Vector3(-4, -4, 4), new Vector3(-4, 4, 4), new Vector3(-4, 4, -4), // left face
		new Vector3(4, -4, -4), new Vector3(-4, -4, -4), new Vector3(-4, 4, -4), new Vector3(4, 4, -4),  // back face
		new Vector3(4, -4, 4), new Vector3(4, -4, -4), new Vector3(4, 4, -4), new Vector3(4, 4, 4),     // right face
		new Vector3(4, 4, 4), new Vector3(4, 4, -4), new Vector3(-4, 4, -4), new Vector3(-4, 4, 4),     // top face
		new Vector3(-4, -4, 4), new Vector3(-4, -4, -4), new Vector3(4, -4, -4), new Vector3(4, -4, 4), // bottom face

		new Vector3(-20, -4, 20), new Vector3(20, -4, 20), new Vector3(20, -4, -20), new Vector3(-20, -4, -20), // grass
		new Vector3(-4, 4, 4), new Vector3(4, 4, 4), new Vector3(0, 8, 4),                       // front attic wall
		new Vector3(4, 4, 4), new Vector3(4, 4, -4), new Vector3(0, 8, -4), new Vector3(0, 8, 4),               // left slope
		new Vector3(-4, 4, 4), new Vector3(0, 8, 4), new Vector3(0, 8, -4), new Vector3(-4, 4, -4),             // right slope
		new Vector3(4, 4, -4), new Vector3(-4, 4, -4), new Vector3(0, 8, -4) };                   // rear attic wall
	static std::vector<Vector3*> * vertexVector = new std::vector<Vector3*>(vertices, vertices + sizeof(vertices) / sizeof(vertices[0]));
	return vertexVector;
}

std::vector<Vector3*>* House::getColors()
{
	return &colorVector;
}

std::vector<GLuint> House::getVertexIndices()
{
	// The index data stores the connectivity of the triangles; 
	// index 0 refers to the first triangle defined above
	static GLuint indices[] = {
		0, 2, 3, 0, 1, 2,      // front face
		4, 6, 7, 4, 5, 6,      // left face
		8, 10, 11, 8, 9, 10,     // back face
		12, 14, 15, 12, 13, 14,   // right face
		16, 18, 19, 16, 17, 18,   // top face
		20, 22, 23, 20, 21, 22,   // bottom face

		24, 26, 27, 24, 25, 26,   // grass
		28, 29, 30,             // front attic wall
		31, 33, 34, 31, 32, 33,   // left slope
		35, 37, 38, 35, 36, 37,   // right slope
		39, 40, 41 };            // rear attic wall	glMatrixMode(GL_MODELVIEW);
	return std::vector<GLuint>(indices, indices + sizeof(indices) / sizeof(indices[0]));
}

void House::initializeColors(void)
{
	// These are the RGB colors corresponding to the vertices, in the same order
	Vector3 * colors[] = {
		new Vector3(1, 0, 0), new Vector3(1, 0, 0), new Vector3(1, 0, 0), new Vector3(1, 0, 0),  // front is red
		new Vector3(0, 1, 0), new Vector3(0, 1, 0), new Vector3(0, 1, 0), new Vector3(0, 1, 0),  // left is green
		new Vector3(1, 0, 0), new Vector3(1, 0, 0), new Vector3(1, 0, 0), new Vector3(1, 0, 0),  // back is red
		new Vector3(0, 1, 0), new Vector3(0, 1, 0), new Vector3(0, 1, 0), new Vector3(0, 1, 0),  // right is green
		new Vector3(0, 0, 1), new Vector3(0, 0, 1), new Vector3(0, 0, 1), new Vector3(0, 0, 1),  // top is blue
		new Vector3(0, 0, 1), new Vector3(0, 0, 1), new Vector3(0, 0, 1), new Vector3(0, 0, 1),  // bottom is blue

		new Vector3(0, 0.5, 0), new Vector3(0, 0.5, 0), new Vector3(0, 0.5, 0), new Vector3(0, 0.5, 0), // grass is dark green
		new Vector3(0, 0, 1), new Vector3(0, 0, 1), new Vector3(0, 0, 1),                // front attic wall is blue
		new Vector3(1, 0, 0), new Vector3(1, 0, 0), new Vector3(1, 0, 0), new Vector3(1, 0, 0),         // left slope is green
		new Vector3(0, 1, 0), new Vector3(0, 1, 0), new Vector3(0, 1, 0), new Vector3(0, 1, 0),         // right slope is red
		new Vector3(0, 0, 1), new Vector3(0, 0, 1), new Vector3(0, 0, 1), };              // rear attic wall is red
	colorVector = std::vector<Vector3*>(colors, colors + sizeof(colors) / sizeof(colors[0]));
}

std::vector<Vector3*>* House::getNormals(void)
{
	return &colorVector;
}

std::vector<GLuint> House::getNormalIndices(void)
{
	return getVertexIndices();
}
