#ifndef BEZIERPATCH_H
#define BEZIERPATCH_H

#include "Vector3.h"
#include "BezierCurve.h"

class BezierPatch
{
protected:
	// p[0..3] corresponds to the first curve along u
	Vector3 p[16];
	Vector3 interpolateUDifferential(float, float);
	Vector3 interpolateVDifferential(float, float);
	BezierCurve *us[4];
	BezierCurve *vs[4];
	void recalculateForPoint(size_t, size_t);

public:
	// Create a bezier patch with the given control points along the u axis
	BezierPatch(Vector3, Vector3, Vector3, Vector3,
		Vector3, Vector3, Vector3, Vector3,
		Vector3, Vector3, Vector3, Vector3,
		Vector3, Vector3, Vector3, Vector3);
	~BezierPatch();

	Vector3 interpolate(float, float);
	// Calculate the normal at the location u, v
	Vector3 interpolateNormal(float, float);
	// u, v, new value. u and v must be within range [0,3]
	void modifyPoint(size_t, size_t, Vector3);
};

#endif // BEZIERPATCH_H
