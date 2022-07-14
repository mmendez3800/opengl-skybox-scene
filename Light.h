#ifndef LIGHT_H
#define LIGHT_H

#include "Matrix4.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Color.h"
#include <vector>
#include "Camera.h"

class Light
{

protected:

	int bindID = -1;
	bool spot;

public:

	Vector4 position;

	Color ambientColor;
	Color diffuseColor;
	Color specularColor;

	float constantAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
	float spot_cutoff;
	float spot_exponent;

public:

	Light(Vector4, bool);
	virtual ~Light(void);

	void bind(int, Camera & camera);
	void unbind(void);
	void setPosition(Vector4);
	Vector4 getPosition();
	void draw(void);
	void updateParameters(Camera & camera);

};

#endif
