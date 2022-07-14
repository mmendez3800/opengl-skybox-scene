#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "Matrix4.h"
#include "Vector4.h"
#include "Vector3.h"

Matrix4::Matrix4()
{
    std::memset(m, 0, sizeof(m));
}

Matrix4::Matrix4(
                 float m00, float m01, float m02, float m03,
                 float m10, float m11, float m12, float m13,
                 float m20, float m21, float m22, float m23,
                 float m30, float m31, float m32, float m33 )
{
    this->set(m00, m01, m02, m03,
              m10, m11, m12, m13,
              m20, m21, m22, m23,
              m30, m31, m32, m33);
}

void Matrix4::set(float m00, float m01, float m02, float m03,
                  float m10, float m11, float m12, float m13,
                  float m20, float m21, float m22, float m23,
                  float m30, float m31, float m32, float m33)
{
    m[0][0] = m00;
    m[0][1] = m01;
    m[0][2] = m02;
    m[0][3] = m03;
    m[1][0] = m10;
    m[1][1] = m11;
    m[1][2] = m12;
    m[1][3] = m13;
    m[2][0] = m20;
    m[2][1] = m21;
    m[2][2] = m22;
    m[2][3] = m23;
    m[3][0] = m30;
    m[3][1] = m31;
    m[3][2] = m32;
    m[3][3] = m33;
}

float Matrix4::get(int column, int element)
{
    return m[column][element];
}

Matrix4& Matrix4::operator=(Matrix4 a)
{
    std::memcpy(m, a.m, sizeof(m));
    return *this;
}

float* Matrix4::ptr()
{
    return &m[0][0];
}

void Matrix4::identity()
{
    static const float ident[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
    std::memcpy(m, ident, sizeof(m));
}

Matrix4 Matrix4::multiply(Matrix4 a)
{
    Matrix4 b;

	for (int column = 0; column < 4; ++column)
	{
		for (int row = 0; row < 4; ++row)
		{
			for (int pair = 0; pair < 4; ++pair)
			{
				b.m[column][row] += a.m[column][pair] * m[pair][row];
			}
		}
	}

	return b;
}

Matrix4 Matrix4::operator * (Matrix4 a)
{
	return multiply(a);
}

Vector4 Matrix4::multiply(Vector4 a)
{
	Vector4 b;

	for (int row = 0; row < 4; ++row)
	{
		for (int pair = 0; pair < 4; ++pair)
		{
			b[row] += m[pair][row] * a[pair];
		}
	}

	return b;
}

Vector4 Matrix4::operator * (Vector4 a)
{
	return multiply(a);
}

Vector3 Matrix4::multiply(Vector3 a)
{
	Vector3 b;

	for (int row = 0; row < 3; ++row)
	{
		for (int pair = 0; pair < 3; ++pair)
		{
			b[row] += m[pair][row] * a[pair];
		}
	}

	return b;
}

Vector3 Matrix4::operator * (Vector3 a)
{
	return multiply(a);
}

Matrix4 Matrix4::makeRotateX(float angle)
{
	identity();

	//Configure this matrix to be a rotation about the X-Axis by 'angle' radians
	m[1][1] = cos(angle);
	m[2][1] = -sin(angle);
	m[1][2] = sin(angle);
	m[2][2] = cos(angle);

	return *this;
}

Matrix4 Matrix4::makeRotateY(float angle)
{
	identity();

	m[0][0] = cos(angle);
	m[0][2] = -sin(angle);
	m[2][0] = sin(angle);
	m[2][2] = cos(angle);

	return *this;
}

Matrix4 Matrix4::makeRotateZ(float angle)
{
	identity();

	m[0][0] = cos(angle);
	m[1][0] = -sin(angle);
	m[0][1] = sin(angle);
	m[1][1] = cos(angle);

	return *this;
}

Matrix4 Matrix4::makeRotateArbitrary(Vector3 a, float angle)
{
	identity();

	Vector3 n = a.normalize();

	this->set(cos(angle) + pow(n[0], 2) * (1 - cos(angle)), n[1] * n[0] * (1 - cos(angle)) + n[2] * sin(angle), n[2] * n[0] * (1 - cos(angle)) - n[1] * sin(angle), 0,
		n[0] * n[1] * (1 - cos(angle)) - n[2] * sin(angle), cos(angle) + pow(n[1], 2) * (1 - cos(angle)), n[2] * n[1] * (1 - cos(angle)) + n[0] * sin(angle), 0,
		n[0] * n[2] * (1 - cos(angle)) + n[1] * sin(angle), n[1] * n[2] * (1-cos(angle)) - n[0] * sin(angle), cos(angle) + pow(n[2], 2) * (1 - cos(angle)), 0,
		0, 0, 0, 1);

	return *this;
}

Matrix4 Matrix4::makeScale(float s)
{
	return makeScale(s, s, s);
}

Matrix4 Matrix4::makeScale(float sx, float sy, float sz)
{
	identity();

	this->set(sx, 0, 0, 0,
		0, sy, 0, 0,
		0, 0, sz, 0,
		0, 0, 0, 1);

	return *this;
}

Matrix4 Matrix4::makeTranslate(float x, float y, float z)
{
	identity();

	this->m[3][0] = x;
	this->m[3][1] = y;
	this->m[3][2] = z;

	return *this;
}

Matrix4 Matrix4::makeTranslate(Vector3 a)
{
	return makeTranslate(a[0], a[1], a[2]);
}

Matrix4 Matrix4::transpose(void)
{
	Matrix4 b;
	for (int x = 0; x < 4; ++x)
	{
		for (int y = 0; y < 4; ++y)
		{
			b.m[y][x] = m[x][y];
		}
	}
	return b;
}

Matrix4 Matrix4::inverse(void)
{
	Matrix4 b;

	return b;
}

Matrix4 Matrix4::rigidInverse(void)
{
	Matrix4 a;
	Matrix4 b;

	a.set(m[0][0], m[0][1], m[0][2], 0, m[1][0], m[1][1], m[1][2], 0, m[2][0], m[2][1], m[2][2], 0, 0, 0, 0, 1);
	a.transpose();
	b.makeTranslate(Vector3(m[3][0], m[3][1], m[3][2]));

	return a*b;
}


Matrix4 Matrix4::makePerspectiveProjection(float fov, float width, float height, float near, float far)
{
	identity();
	fov = fov / 180 * M_PI;

	float aspect = width / height;

	set(1.0f / (aspect * tan(fov / 2.0f)), 0, 0, 0,
		0, 1.0f/tan(fov/2.0f), 0, 0,
		0, 0, (near + far) / (near - far), -1.0,
		0, 0, 2.0f*near*far / (near - far), 0);

	return *this;
}

Matrix4 Matrix4::makeViewport(float xmin, float xmax, float ymin, float ymax)
{
	identity();

	set((xmax - xmin) / 2, 0, 0, 0,
		0, (ymax - ymin) / 2, 0, 0,
		0, 0, 0.5f, 0,
		(xmin + xmax) / 2.0f, (ymin + ymax) / 2.0f, 0.5f, 1.0f);

	return *this;
}

void Matrix4::print(std::string comment)
{
	//Width constants and variables
	static const int pointWidth = 1;
	static const int precisionWidth = 4;
	int integerWidth = 1;

	//Determine the necessary width to the left of the decimal point
	float* elementPtr = (float*)m;
	float maxValue = fabsf(*(elementPtr++));
	while (elementPtr++ < ((float*)m + 16)) if (fabsf(*elementPtr) > maxValue) maxValue = fabsf(*elementPtr);
	while (maxValue >= 10.0) { ++integerWidth; maxValue /= 10.0; }

	//Sum up the widths to determine the cell width needed
	int cellWidth = integerWidth + pointWidth + precisionWidth;

	//Set the stream parameters for fixed number of digits after the decimal point
	//and a set number of precision digits
	std::cout << std::fixed;
	std::cout << std::setprecision(precisionWidth);

	//Print the comment
	std::cout << comment << std::endl;

	//Loop through the matrix elements, format each, and print them to screen
	float cellValue;
	for (int element = 0; element < 4; element++)
	{
		std::cout << std::setw(1) << (element == 0 ? "[" : " ");
		for (int vector = 0; vector < 4; vector++)
		{
			cellValue = m[vector][element];
			std::cout << std::setw(cellWidth + (cellValue >= 0.0 ? 1 : 0)) << cellValue;
			std::cout << std::setw(0) << (vector < 3 ? " " : "");
		}
		std::cout << std::setw(1) << (element == 3 ? "]" : " ") << std::endl;
	}
}

Matrix4 Matrix4::makeOrtho(float left, float right, float bottom, float top, float near, float far)
{
	float tx = -(right + left) / (right - left);
	float ty = -(top + bottom) / (top - bottom);
	float tz = -(far + near) / (far - near);
	this->set(2 / (right - left), 0, 0, 0,
		0, 2 / (top - bottom), 0, 0,
		0, 0, -2 / (far - near), 0,
		tx, ty, tz, 1);

	return *this;
}
