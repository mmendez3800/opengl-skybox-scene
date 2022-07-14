#include "MatrixTransform.h"
#include <GL/freeglut.h>

MatrixTransform::MatrixTransform(void)
{
	transform.identity();
}

MatrixTransform::MatrixTransform(Matrix4 T)
{
	transform = T;
}

void MatrixTransform::draw(Matrix4 & C, ViewFrustum& frust)
{
	Matrix4 transformed = C * transform;
	Group::draw(transformed, frust);
}

void MatrixTransform::preMultMatrix(Matrix4 & mat)
{
	transform = mat * transform;
}

void MatrixTransform::postMultMatrix(Matrix4 & mat)
{
	transform = transform * mat;
}

void MatrixTransform::setMatrix(Matrix4 mat)
{
	transform = mat;
}

Matrix4 MatrixTransform::getMatrix(void)
{
	return transform;
}

BoundBox MatrixTransform::update(void)
{
	BoundBox box = Group::update();
	box.transform(transform);
	box.standardize();
	box.calculateSphere(bound_point, bound_radius);
	return box;
}

void MatrixTransform::drawBoundingSphere(Matrix4 & C)
{
	Matrix4 trans = C * transform;
	glPushMatrix();
	glMultMatrixf(transform.ptr());
	Group::drawBoundingSphere(trans);
	glPopMatrix();
}
