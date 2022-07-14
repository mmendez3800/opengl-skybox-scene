#include "BezierCurve.h"
#include <iostream>

BezierCurve::BezierCurve(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3)
{
	p[0] = p0;
	p[1] = p1;
	p[2] = p2;
	p[3] = p3;
	recalculateValues();
}

Vector3 BezierCurve::interpolate(float t)
{
	return a.scale(t*t*t) + b.scale(t*t) + c.scale(t) + d;
}

Vector3 BezierCurve::interpolateDifferential(float t)
{
	return a.scale(3 * t * t) + b.scale(2 * t) + c;
}

void BezierCurve::recalculateValues(void)
{
	a = p[0].scale(-1) + p[1].scale(3) + p[2].scale(-3) + p[3];
	b = p[0].scale(3) + p[1].scale(-6) + p[2].scale(3);
	c = p[0].scale(-3) + p[1].scale(3);
	d = p[0];
}

void BezierCurve::modifyPoint(size_t t, Vector3 p)
{
	if (t < 0 || t > 3)
	{
		std::cerr << "Invalid bezier curve modification" << std::endl;
		return;
	}
	this->p[t] = p;
}
