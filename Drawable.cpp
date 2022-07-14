#include "Window.h"
#include "Drawable.h"
#include <algorithm>

Drawable::Drawable()
{
    this->toWorld.identity();
	this->transform.identity();
	this->scale.identity();
	this->rotate.identity();
	this->needs_update = false;
}

Drawable::~Drawable()
{
    //
}

void Drawable::draw(DrawData& data)
{
    std::cout << "Warning: draw() is not implemented" << std::endl;
}

void Drawable::update(UpdateData& data)
{
    std::cout << "Warning: update() is not implemented" << std::endl;
}

/*
std::vector<Vector3*>* Drawable::getVertices(void)
{
    std::cout << "Warning: getVertices() is not implemented" << std::endl;
	return nullptr;
}*/

std::vector<GLuint> Drawable::getVertexIndices(void)
{
	std::cout << "Warning: getVertexIndices() is not implemented" << std::endl;
	return std::vector<GLuint>();
}

/*
std::vector<Vector3*>* Drawable::getNormals(void)
{
	std::cout << "Warning: getNormals() is not implemented" << std::endl;
	return nullptr;
}
*/

std::vector<GLuint> Drawable::getNormalIndices(void)
{
	std::cout << "Warning: getNormalIndices() is not implemented" << std::endl;
	return std::vector<GLuint>();
}

/*
void Drawable::center(void)
{
	//std::vector<Vector3*>* vertices = getVertices();
	Vector3 seed_vec = toWorld * *vertices->at(0);
	float left = seed_vec[0];
	float right = seed_vec[0];
	float top = seed_vec[1];
	float bottom = seed_vec[1];

	std::vector<Vector3*>::iterator itr = vertices->begin();
	while (itr != vertices->end())
	{
		Vector3 vec = (toWorld * (*itr++)->toVector4(1.0f)).toVector3();
		left = min(vec.operator[](0), left);
		right = max(vec.operator[](0), right);
		top = max(vec.operator[](1), top);
		bottom = min(vec.operator[](1), bottom);
	}

	float xcenter = (right + left) / 2;
	float ycenter = (top + bottom) / 2;

	Matrix4 translate;
	translate.makeTranslate(Vector3(-xcenter, -ycenter, 0.0f));

	toWorld = translate * toWorld;
}
*/

void Drawable::scaleToFit(float factor)
{
	std::cout << "no scaling implemented for this object." << std::endl;
}

float Drawable::getScaleFactor(void)
{
	return 1.0f;
}
