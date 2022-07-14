#ifndef BOUNDBOX_H
#define BOUNDBOX_H

#include "Vector4.h"
#include "Vector3.h"
#include "Matrix4.h"

class BoundBox
{
private:
	Vector4 p1, p2, p3, p4, p5, p6, p7, p8;

public:
	BoundBox(void);
	BoundBox(Vector4&, Vector4&, Vector4&, Vector4&, Vector4&, Vector4&, Vector4&, Vector4&);
	BoundBox(Vector3&, Vector3&, Vector3&, Vector3&, Vector3&, Vector3&, Vector3&, Vector3&);
	void transform(Matrix4 &);
	void calculateSphere(Vector3&, float&);
	void growToFit(BoundBox&);
	void standardize(void);
};

#endif // BOUNDBOX_H
