#include "BoundBox.h"
#include <algorithm>

static float avg(float a, float b, float c, float d, float e, float f, float g, float h)
{
	return (a + b + c + d + e + f + g + h ) / 8.0f;
}

static float dist(Vector3 a, Vector4 p)
{
	return (p.toVector3() - a).magnitude();
}

BoundBox::BoundBox(void)
{
}

BoundBox::BoundBox(Vector4& p1, Vector4& p2, Vector4& p3, Vector4& p4, Vector4& p5, Vector4& p6, Vector4& p7, Vector4& p8) : p1(p1), p2(p2), p3(p3), p4(p4), p5(p5), p6(p6), p7(p7), p8(p8)
{
	standardize();
}

BoundBox::BoundBox(Vector3& p1, Vector3& p2, Vector3& p3, Vector3& p4, Vector3& p5, Vector3& p6, Vector3& p7, Vector3& p8) :
p1(p1.toVector4(1.0f)), p2(p2.toVector4(1.0f)), p3(p3.toVector4(1.0f)), p4(p4.toVector4(1.0f)), p5(p5.toVector4(1.0f)), p6(p6.toVector4(1.0f)), p7(p7.toVector4(1.0f)), p8(p8.toVector4(1.0f))
{
	standardize();
}

void BoundBox::transform(Matrix4& T)
{
	p1 = T * p1;
	p2 = T * p2;
	p3 = T * p3;
	p4 = T * p4;
	p5 = T * p5;
	p6 = T * p6;
	p7 = T * p7;
	p8 = T * p8;
}

void BoundBox::calculateSphere(Vector3 & center, float & radius)
{
	center[0] = avg(p1[0], p2[0], p3[0], p4[0], p5[0], p6[0], p7[0], p8[0]);
	center[1] = avg(p1[1], p2[1], p3[1], p4[1], p5[1], p6[1], p7[1], p8[1]);
	center[2] = avg(p1[2], p2[2], p3[2], p4[2], p5[2], p6[2], p7[2], p8[2]);

	radius = std::max(std::max(std::max(std::max(std::max(std::max(std::max(dist(center, p1), dist(center, p2)),
		dist(center, p3)), dist(center, p4)), dist(center, p5)), dist(center, p6)), dist(center, p7)), dist(center, p8));
}

void BoundBox::growToFit(BoundBox& fitMe)
{
	// left top front
	p1[0] = std::min(p1[0], fitMe.p1[0]);
	p1[1] = std::max(p1[1], fitMe.p1[1]);
	p1[2] = std::max(p1[2], fitMe.p1[2]);

	// right top front
	p2[0] = std::max(p2[0], fitMe.p2[0]);
	p2[1] = std::max(p2[1], fitMe.p2[1]);
	p2[2] = std::max(p2[2], fitMe.p2[2]);

	// left bottom front
	p3[0] = std::min(p3[0], fitMe.p3[0]);
	p3[1] = std::min(p3[1], fitMe.p3[1]);
	p3[2] = std::max(p3[2], fitMe.p3[2]);

	// right bottom front
	p4[0] = std::max(p4[0], fitMe.p4[0]);
	p4[1] = std::min(p4[1], fitMe.p4[1]);
	p4[2] = std::max(p4[2], fitMe.p4[2]);

	// left top back
	p5[0] = std::min(p5[0], fitMe.p5[0]);
	p5[1] = std::max(p5[1], fitMe.p5[1]);
	p5[2] = std::min(p5[2], fitMe.p5[2]);

	// right top back
	p6[0] = std::max(p6[0], fitMe.p6[0]);
	p6[1] = std::max(p6[1], fitMe.p6[1]);
	p6[2] = std::min(p6[2], fitMe.p6[2]);

	// left bottom back
	p7[0] = std::min(p7[0], fitMe.p7[0]);
	p7[1] = std::min(p7[1], fitMe.p7[1]);
	p7[2] = std::min(p7[2], fitMe.p7[2]);

	// right bottom back
	p8[0] = std::max(p8[0], fitMe.p8[0]);
	p8[1] = std::min(p8[1], fitMe.p8[1]);
	p8[2] = std::min(p8[2], fitMe.p8[2]);
}

void BoundBox::standardize(void)
{
	float left = std::min(std::min(std::min(std::min(std::min(std::min(std::min(p1[0], p2[0]), p3[0]), p4[0]), p5[0]), p6[0]), p7[0]), p8[0]);
	float right = std::max(std::max(std::max(std::max(std::max(std::max(std::max(p1[0], p2[0]), p3[0]), p4[0]), p5[0]), p6[0]), p7[0]), p8[0]);
	float bottom = std::min(std::min(std::min(std::min(std::min(std::min(std::min(p1[1], p2[1]), p3[1]), p4[1]), p5[1]), p6[1]), p7[1]), p8[1]);
	float top = std::max(std::max(std::max(std::max(std::max(std::max(std::max(p1[1], p2[1]), p3[1]), p4[1]), p5[1]), p6[1]), p7[1]), p8[1]);
	float back = std::min(std::min(std::min(std::min(std::min(std::min(std::min(p1[2], p2[2]), p3[2]), p4[2]), p5[2]), p6[2]), p7[2]), p8[2]);
	float front = std::max(std::max(std::max(std::max(std::max(std::max(std::max(p1[2], p2[2]), p3[2]), p4[2]), p5[2]), p6[2]), p7[2]), p8[2]);

	p1 = Vector4(left, top, front, 1.0f);
	p2 = Vector4(right, top, front, 1.0f);
	p3 = Vector4(left, bottom, front, 1.0f);
	p4 = Vector4(right, bottom, front, 1.0f);
	p5 = Vector4(left, top, back, 1.0f);
	p6 = Vector4(right, top, back, 1.0f);
	p7 = Vector4(left, bottom, back, 1.0f);
	p8 = Vector4(right, bottom, back, 1.0f);
}
