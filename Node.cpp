#include "Node.h"

#include <GL/freeglut.h>

void Node::forceDrawBoundSphere(Matrix4 & C)
{
	static float colors[] = { 1.0f, 1.0f, 1.0f };
	glPushMatrix();
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, colors);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, colors);
	Matrix4 move;
	// Translate sphere to world coordinates
	move.makeTranslate(bound_point);
	// Translate sphere to camera coordinates
	glMultMatrixf(move.ptr());
	glutWireSphere(bound_radius, 10, 10);
	glPopMatrix();
}

void Node::drawBoundingSphere(Matrix4 & C)
{
	forceDrawBoundSphere(C);
}

void Node::checkDraw(Matrix4& C, ViewFrustum& frust)
{
	if (frust.insideOrIntersect((C*(bound_point.toVector4(1.0f))).toVector3(), bound_radius))
	{
		draw(C, frust);
	}
}
