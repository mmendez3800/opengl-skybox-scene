#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <iostream>
#include "Matrix4.h"
#include "DrawData.h"
#include "UpdateData.h"
#include "Material.h"
#include <float.h>
#include <math.h>
#include <vector>
#include <GL/glut.h>
#include <algorithm>
#include "Vector4.h"


class Drawable
{
    
public:
    
	Matrix4 scale;
	Matrix4 transform;
    Matrix4 toWorld;
	Matrix4 rotate;
	bool needs_update;
    Material material;
    
    Drawable(void);
    ~Drawable(void);
    
    virtual void draw(DrawData&);
    virtual void update(UpdateData&);
    
	//virtual std::vector<Vector3*>* getVertices();
	//virtual std::vector<Vector3*>* getNormals();
	// Unneeded
	//virtual std::vector<Vector3*>* getColors();
	virtual std::vector<GLuint> getVertexIndices();
	virtual std::vector<GLuint> getNormalIndices();

	//virtual void center();
	virtual void scaleToFit(float factor);
	virtual float getScaleFactor(void);
};

#endif
