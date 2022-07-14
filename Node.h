#ifndef NODE_H
#define NODE_H

#include "Matrix4.h"
#include "Vector3.h"
#include "BoundBox.h"
#include "ViewFrustum.h"

class Node
{
protected:
	Vector3 bound_point;
	float bound_radius;

public:
	// draw
	virtual void draw(Matrix4&, ViewFrustum&) = 0;

	void checkDraw(Matrix4&, ViewFrustum&);

	// update bounding sphere
	virtual BoundBox update(void) = 0;
	
	virtual void clearChildren(bool) = 0;

	virtual void drawBoundingSphere(Matrix4 &);
	void forceDrawBoundSphere(Matrix4 &);
};

#endif // NODE_H
