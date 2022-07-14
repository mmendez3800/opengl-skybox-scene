#include "CubeNode.h"

#include <GL/freeglut.h>

CubeNode::CubeNode(float length) : length(length)
{
	material.ambientColor = Color(0.7f, 1.0f, 0.9f);
}

void CubeNode::render(void)
{
	material.apply();
	glutSolidCube(length);
}

BoundBox CubeNode::update(void)
{
	float hl = length / 2.0f;
	BoundBox box(
		Vector3(hl, hl, hl),
		Vector3(hl, hl, -hl),
		Vector3(hl, -hl, hl),
		Vector3(hl, -hl, -hl),
		Vector3(-hl, hl, hl),
		Vector3(-hl, hl, -hl),
		Vector3(-hl, -hl, hl),
		Vector3(-hl, -hl, -hl)
		);
	box.calculateSphere(bound_point, bound_radius);
	return box;
}
