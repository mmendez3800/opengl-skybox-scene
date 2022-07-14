#include "ConeNode.h"
#include <GL/freeglut.h>

ConeNode::ConeNode(float base, float height) : base(base), height(height)
{
}

void ConeNode::render(void)
{
	static float color[] = {0.7f, 0.6f, 0.7f};
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glutSolidCone(base, height, 10, 10);
}

BoundBox ConeNode::update(void)
{
	float top = height;
	float bottom = -height;
	float left = base / 2.0f;
	float right = base / 2.0f;
	float front = base / 2.0f;
	float back = base / 2.0f;
	BoundBox box(
		Vector3(left, top, front),
		Vector3(right, top, front),
		Vector3(left, top, back),
		Vector3(right, top, back),
		Vector3(left, bottom, front),
		Vector3(right, bottom, front),
		Vector3(left, bottom, back),
		Vector3(right, bottom, back)
		);
	box.calculateSphere(bound_point, bound_radius);
	return box;
}
