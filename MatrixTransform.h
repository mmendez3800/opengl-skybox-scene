#ifndef MATRIXTRANSFORM_H
#define MATRIXTRANSFORM_H

#include "Group.h"

class MatrixTransform : public Group
{
private:
	Matrix4 transform;

public:
	MatrixTransform(void);
	MatrixTransform(Matrix4);

	void preMultMatrix(Matrix4 &);
	void postMultMatrix(Matrix4 &);
	void setMatrix(Matrix4);
	Matrix4 getMatrix(void);
	virtual BoundBox update(void);
	virtual void draw(Matrix4&, ViewFrustum&);
	virtual void drawBoundingSphere(Matrix4 &);
};

#endif // MATRIXTRANSFORM_H
