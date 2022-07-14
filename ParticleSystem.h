#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include "Shader.h"
#include "Vector3.h"
#include "UpdateData.h"
#include "Camera.h"
#include "Vector4.h"

struct Particle {
	Vector3 position;
	Vector3 velocity;
	Vector3 color;
	float amountAlive;
	float lifetime;
	float weight;
	float size;
};

class ParticleSystem
{
private:
	Shader * shader;

	void checkInitShader(void);
	void checkDeleteShader(void);
	static int instance_counter;

public:
	Particle particleCollection[9000];
	float moveTime;
	float angleOfSpray;
	Vector3 center;
	int stop;

	ParticleSystem(float, Vector3);
	~ParticleSystem();

	void drawParticle(Particle*, Vector3);
	virtual void draw(Camera&, Matrix4&, Vector4&);
	Vector3 currentVelocity();
	void move();
	void callMove(float);
	void update(UpdateData&);
	float randomNumber();
};

#endif