#include "Light.h"
#include "Window.h"
#include <iostream>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif


Light::Light(Vector4 position, bool spot)
{
	this->position = position;
	this->spot = spot;
	ambientColor = Color::ambientDefault();
	diffuseColor = Color::diffuseDefault();
	specularColor = Color::specularDefault();

	constantAttenuation = 1.0f;
	linearAttenuation = 0.0f;
	quadraticAttenuation = 0.05f;
	if (spot)
	{
		spot_cutoff = 30.0f;
		spot_exponent = 1.0f;
	}
}

Light::~Light()
{
	//Delete any dynamically allocated memory/objects here
}

void Light::bind(int id, Camera & camera)
{
	if (id < 0 || id > 7) {
		std::cout << "ERROR: The light bind ID " << id << " is not valid!" << std::endl;
		return;
	}

	//Set the bindID
	bindID = id;

	//Configure the light at the bindID
	glEnable(GL_LIGHT0 + bindID);

	//Configure the color of the light
	glLightfv(GL_LIGHT0 + bindID, GL_AMBIENT, ambientColor.ptr());
	glLightfv(GL_LIGHT0 + bindID, GL_DIFFUSE, diffuseColor.ptr());
	glLightfv(GL_LIGHT0 + bindID, GL_SPECULAR, specularColor.ptr());

	//Configure the attenuation properties of the light
	//Add support for Constant Attenuation
	//Add support for Linear Attenuation
	glLightf(GL_LIGHT0 + bindID, GL_QUADRATIC_ATTENUATION, quadraticAttenuation);

	updateParameters(camera);
}

void Light::updateParameters(Camera &camera)
{
	Vector4 pos = position;
	if (pos[3] != 0)
	{
		pos = camera.getInverseMatrix() * pos;
	}
	//Position the light
	glLightfv(GL_LIGHT0 + bindID, GL_POSITION, pos.ptr());

	//Setup spotlight direction, angle, and exponent here
	if (spot)
	{
		//Vector3 direction(0.0f, 0.0f, -1.0f);
		//direction = camera.getInverseMatrix() * direction;
		Vector3 direction = position.toVector3().normalize();
		direction = direction.scale(-1.0f).normalize();
		glLightfv(GL_LIGHT0 + bindID, GL_SPOT_DIRECTION, direction.ptr());
		glLightf(GL_LIGHT0 + bindID, GL_SPOT_CUTOFF, spot_cutoff);
		glLightf(GL_LIGHT0 + bindID, GL_SPOT_EXPONENT, spot_exponent);
	}
}

void Light::unbind(void)
{
	glDisable(GL_LIGHT0 + bindID);
	bindID = -1;
}

void Light::setPosition(Vector4 position)
{
	this->position = position;
}

Vector4 Light::getPosition()
{
	return position;
}

void Light::draw()
{
	static float emission[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	static float others[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	Matrix4 toWorld;
	if (!spot)
		toWorld.makeTranslate(position[0], position[1], position[2]);
	else
		toWorld.makeTranslate(position[0], position[1], position[2]);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glMultMatrixf(toWorld.ptr());

	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
	glMaterialfv(GL_FRONT, GL_SPECULAR, others);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, others);

	glutSolidSphere(1.0, 10, 10);

	glPopMatrix();
}
