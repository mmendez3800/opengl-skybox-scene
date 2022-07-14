#include "Vector2.h"
#include <math.h>
#include <iostream>
#include <cstring>

Vector2::Vector2()
{
	std::memset(m, 0, sizeof(m));
}

Vector2::Vector2(float m0, float m1)
{
	m[0] = m0;
	m[1] = m1;
}

Vector2::~Vector2()
{
	//
}

float* Vector2::ptr()
{
	return &m[0];
}

void Vector2::set(float x, float y)
{
	m[0] = x;
	m[1] = y;
}

void Vector2::set(int index, float value)
{
	m[index] = value;
}

float& Vector2::operator [] (int loc)
{
	return m[loc];
}

void Vector2::print(std::string comment)
{
	std::cout << comment << std::endl;
	std::cout << "<x:" << m[0] << ", y:" << m[1] << ", z:" << m[2] << ">" << std::endl;
}
