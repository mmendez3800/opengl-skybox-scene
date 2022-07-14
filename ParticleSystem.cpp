#include "ParticleSystem.h"
#include <vector>
#include "ErrorHandling.h"

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/GLee.h>
	#include <GL/glut.h>
#endif

int numberOfParticles = 9000;
float useMove = 0;
float size = 2;

int ParticleSystem::instance_counter(0);

ParticleSystem::ParticleSystem(float angleOfSpray, Vector3 center)
{
	ErrorHandling::testGlError(__FILE__, __LINE__);
	checkInitShader();
	this->moveTime = 0.01f;
	this->angleOfSpray = angleOfSpray;
	this->center = center;
	this->stop = 1;

	for (int i = 0; i < numberOfParticles; i++) {
		drawParticle(particleCollection + i, this->center);
		move();
	}
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

ParticleSystem::~ParticleSystem()
{
	checkDeleteShader();
}

void ParticleSystem::checkInitShader(void)
{
	ErrorHandling::testGlError(__FILE__, __LINE__);
	if (instance_counter == 0)
	{
		shader = new Shader("particle.vert", "particle.frag", true);
	}
	++instance_counter;
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

void ParticleSystem::checkDeleteShader(void)
{
	if (--instance_counter == 0)
	{
		delete shader;
		shader = nullptr;
	}
}

void ParticleSystem::drawParticle(Particle* particle, Vector3 position)
{
	particle->position = position;
	particle->velocity = currentVelocity() + Vector3(0.5f * randomNumber() - 0.25f, 0.5f * randomNumber() - 0.25f, 0.5f * randomNumber() - 0.25f);
	particle->color = Vector3(1, 1, 1);
	particle->amountAlive = 0;
	particle->lifetime = randomNumber() + 1;
	particle->weight = 3;
}

void ParticleSystem::draw(Camera & cam, Matrix4 & P, Vector4 & clip)
{
	ErrorHandling::testGlError(__FILE__, __LINE__);
	shader->bind();
	GLuint color_location = shader->getAttribLocation("particle_color");
	glUniformMatrix4fv(shader->getUniformLocation("cam_inv"), 1, GL_FALSE, cam.getInverseMatrix().ptr());
	glUniformMatrix4fv(shader->getUniformLocation("proj"), 1, GL_FALSE, P.ptr());
	glUniform4fv(shader->getUniformLocation("clip"), 1, clip.ptr());
	glPointSize(size);
	glTexEnvf(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	glEnable(GL_POINT_SPRITE);
	glBegin(GL_POINTS);
	for (int i = 0; i < numberOfParticles; i++) {
		Particle* particle = particleCollection + i;

		//glColor4f(particle->color[0], particle->color[1], particle->color[2], (1 - particle->amountAlive / particle->lifetime));
		glVertexAttrib4f(color_location, particle->color[0], particle->color[1], particle->color[2], (1 - particle->amountAlive / particle->lifetime));

		Vector3 position = particle->position;
		glVertex3f(position[0], position[1], position[2]);
	}
	glEnd();
	glDisable(GL_POINT_SPRITE);
	shader->unbind();
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

Vector3 ParticleSystem::currentVelocity()
{
	return Vector3(2 * cos(angleOfSpray), 0, 2 * sin(angleOfSpray));
}

void ParticleSystem::move()
{
	for (int i = 0; i < numberOfParticles; i++) {
		Particle* particle = particleCollection + i;

		particle->position = particle->position + particle->velocity * moveTime;
		particle->velocity = particle->velocity + Vector3(0.0f, -particle->weight * moveTime, 0);
		particle->amountAlive = particle->amountAlive + moveTime;

		if (particle->amountAlive > particle->lifetime && stop == 1) {
			drawParticle(particle, center);
		}
	}
}

void ParticleSystem::callMove(float time)
{
	while (time > 0) {
		if (useMove < time) {
			time = time - useMove;
			move();
			useMove = this->moveTime;
		}

		else {
			useMove = useMove - time;
			time = 0;
		}
	}
}

void ParticleSystem::update(UpdateData& data)
{
	this->callMove(data.dt);
}

float ParticleSystem::randomNumber()
{
	return (float)rand() / ((float)RAND_MAX);
}