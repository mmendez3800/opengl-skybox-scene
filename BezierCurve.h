#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include "Vector3.h"

class BezierCurve
{
private:
	Vector3 a, b, c, d;
	Vector3 p[4];
	void recalculateValues(void);

public:
	BezierCurve(Vector3, Vector3, Vector3, Vector3);

	Vector3 interpolate(float t);
	Vector3 interpolateDifferential(float t);

	void modifyPoint(size_t, Vector3);
};

#endif // BEZIERCURVE_H
