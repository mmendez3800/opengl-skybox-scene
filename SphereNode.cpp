#include "SphereNode.h"
#include <GL/freeglut.h>

SphereNode::SphereNode(float radius) : radius(radius)
{
	material.ambientColor = Color(1.0f, 0.5, 0.5f);
}

void SphereNode::render(void)
{
	material.apply();
	glutSolidSphere(radius, 10, 10);
}

BoundBox SphereNode::update(void)
{
	bound_point = Vector3(0.0f, 0.0f, 0.0f);
	bound_radius = radius;
	BoundBox box(
		Vector3(radius, radius, radius),
		Vector3(radius, radius, -radius),
		Vector3(radius, -radius, radius),
		Vector3(radius, -radius, -radius),
		Vector3(-radius, radius, radius),
		Vector3(-radius, radius, -radius),
		Vector3(-radius, -radius, radius),
		Vector3(-radius, -radius, -radius)
		);
	return box;
}
