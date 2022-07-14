#ifndef HOUSE_H
#define HOUSE_H

#include "Vector4.h"
#include "Drawable.h"
#include "Camera.h"
#include "DrawData.h"
#include <GL/glut.h>

class House : public Drawable
{
private:
	std::vector<Vector3*> normals;
	std::vector<Vector3*> colorVector;

	void initializeColors(void);
	void initializeVertices(void);

public:
	House(void);
	virtual void draw(DrawData&);

	virtual std::vector<Vector3*>* getVertices(void);
	virtual std::vector<GLuint> getVertexIndices(void);
	virtual std::vector<Vector3*>* getColors(void);
	virtual std::vector<Vector3*>* getNormals(void);
	virtual std::vector<GLuint> getNormalIndices(void);
};

#endif