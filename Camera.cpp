#include "Camera.h"

Camera::Camera()
{
    c.identity();
    e.set(0.0, 0.0, 20.0);
    d.set(0.0, 0.0, 0.0);
    up.set(0.0, 1.0, 0.0);
    
    //Pre-define a camera matrix (and its inverse) that are shifted 'e' from the origin
    //This is used as a default camera position for Project 1
    c.set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, e[0], e[1], e[2], 1);
    ci.set(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -e[0], -e[1], -e[2], 1);
}

Camera::~Camera()
{
    //Delete and dynamically allocated memory/objects here
}

Matrix4& Camera::getMatrix()
{
    return c;
}

Matrix4& Camera::getInverseMatrix()
{
    return ci;
}

void Camera::update()
{
    //Update the Camera Matrix using d, e, and up
    //Solve for the z, x, and y axes of the camera matrix
    //Use these axes and the e vector to create a camera matrix c
    //Use c to solve for an inverse camera matrix ci
	Vector3 z = (e - d).normalize();
	Vector3 x = up.cross(z).normalize();
	//Vector3 y = up.normalize();
	Vector3 y = z.cross(x);
	Matrix4 rotation = Matrix4(x[0], x[1], x[2], 0,
		y[0], y[1], y[2], 0,
		z[0], z[1], z[2], 0,
		0, 0, 0, 1);
	Matrix4 translation = Matrix4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-e[0], -e[1], -e[2], 1);
	this->c.set(x[0], x[1], x[2], 0,
		y[0], y[1], y[2], 0,
		z[0], z[1], z[2], 0,
		e[0], e[1], e[2], 1
		);
	/*
	this->ci.set(x[0], y[0], z[0], 0,
		x[1], y[1], z[1], 0,
		x[2], y[2], z[2], 0,
		-e[0], -e[1], -e[2], 1);
		*/
	this->ci = rotation.transpose() * translation;
}

void Camera::set(Vector3& e, Vector3& d, Vector3& up)
{
    this->e = e;
    this->d = d;
    this->up = up;
    update();
}

void Camera::reflectAcrossZ(void)
{
	this->e.set(2, -e[2]);
	update();
}

void Camera::reflectAcrossY(void)
{
	e.set(1, -e[1]);
	update();
}

void Camera::cloneInto(Camera & c)
{
	c.e = e;
	c.d = d;
	c.up = up;
	c.update();
}
