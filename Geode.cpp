#include "Geode.h"
#include <GL/freeglut.h>

void Geode::draw(Matrix4 & C, ViewFrustum& frust)
{
	glPushMatrix();
	glMultMatrixf(C.ptr());
	render();
	glPopMatrix();
}

void Geode::clearChildren(bool)
{
	// ignored
}
