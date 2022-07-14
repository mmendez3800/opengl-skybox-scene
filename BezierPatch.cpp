#include "BezierPatch.h"

BezierPatch::BezierPatch(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3,
	Vector3 p4, Vector3 p5, Vector3 p6, Vector3 p7,
	Vector3 p8, Vector3 p9, Vector3 p10, Vector3 p11,
	Vector3 p12, Vector3 p13, Vector3 p14, Vector3 p15
	)
{
	// because arrays are stupid and I don't know why I'm using one
	p[0] = p0;
	p[1] = p1;
	p[2] = p2;
	p[3] = p3;
	p[4] = p4;
	p[5] = p5;
	p[6] = p6;
	p[7] = p7;
	p[8] = p8;
	p[9] = p9;
	p[10] = p10;
	p[11] = p11;
	p[12] = p12;
	p[13] = p13;
	p[14] = p14;
	p[15] = p15;
	us[0] = new BezierCurve(p0, p1, p2, p3);
	us[1] = new BezierCurve(p4, p5, p6, p7);
	us[2] = new BezierCurve(p8, p9, p10, p11);
	us[3] = new BezierCurve(p12, p13, p14, p15);
	vs[0] = new BezierCurve(p0, p4, p8, p12);
	vs[1] = new BezierCurve(p1, p5, p9, p13);
	vs[2] = new BezierCurve(p2, p6, p10, p14);
	vs[3] = new BezierCurve(p3, p7, p11, p15);
}

Vector3 BezierPatch::interpolate(float u, float v)
{
	BezierCurve n(us[0]->interpolate(u), us[1]->interpolate(u), us[2]->interpolate(u), us[3]->interpolate(u));
	return n.interpolate(v);
}

Vector3 BezierPatch::interpolateVDifferential(float u, float v)
{
	BezierCurve n(us[0]->interpolate(u), us[1]->interpolate(u), us[2]->interpolate(u), us[3]->interpolate(u));
	return n.interpolateDifferential(v);
}

Vector3 BezierPatch::interpolateUDifferential(float u, float v)
{
	BezierCurve n(vs[0]->interpolate(v), vs[1]->interpolate(v), vs[2]->interpolate(v), vs[3]->interpolate(v));
	return n.interpolateDifferential(u);
}

Vector3 BezierPatch::interpolateNormal(float u, float v)
{
	Vector3 u_first = interpolateVDifferential(u, v);
	Vector3 v_first = interpolateUDifferential(u, v);
	return u_first.cross(v_first).normalize();
}

BezierPatch::~BezierPatch(void)
{
	delete us[0];
	delete us[1];
	delete us[2];
	delete us[3];
	delete vs[0];
	delete vs[1];
	delete vs[2];
	delete vs[3];
}

void BezierPatch::modifyPoint(size_t u, size_t v, Vector3 p)
{
	this->p[u * 4 + v] = p;
	us[u]->modifyPoint(v, p);
	vs[v]->modifyPoint(u, p);
}
