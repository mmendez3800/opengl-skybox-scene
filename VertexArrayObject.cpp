#include <GL/GLee.h>
#include "VertexArrayObject.h"
#include "ErrorHandling.h"

GLuint VertexArrayObject::currently_bound(0);

VertexArrayObject::VertexArrayObject(void)
{
	glGenVertexArrays(1, &vao_id);
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

VertexArrayObject::~VertexArrayObject(void)
{
	glDeleteVertexArrays(1, &vao_id);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	for (std::vector<GLuint>::iterator itr = buffer_objects.begin(); itr != buffer_objects.end(); ++itr)
	{
		glDeleteBuffers(1, &*itr);
	}
}

void VertexArrayObject::bind(void)
{
	if (currently_bound != vao_id)
	{
		glBindVertexArray(vao_id);
		ErrorHandling::testGlError(__FILE__, __LINE__);
		currently_bound = vao_id;
	}
}

void VertexArrayObject::unbind(void)
{
	if (currently_bound == vao_id)
	{
		glBindVertexArray(0);
	}
}

void VertexArrayObject::enableVertexAttribArray(GLuint index)
{
	if (currently_bound != vao_id)
	{
		glBindVertexArray(vao_id);
		ErrorHandling::testGlError(__FILE__, __LINE__);
		glEnableVertexAttribArray(index);
		ErrorHandling::testGlError(__FILE__, __LINE__);
		glBindVertexArray(currently_bound);
		ErrorHandling::testGlError(__FILE__, __LINE__);
	}
	else
	{
		glEnableVertexAttribArray(index);
		ErrorHandling::testGlError(__FILE__, __LINE__);
	}
}

void VertexArrayObject::bindDataToAttribute(GLuint attribute, GLint element_size, float * data, size_t length)
{
	doBindCheck();
	GLuint vbo;
	glGenBuffers(1, &vbo);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glBufferData(GL_ARRAY_BUFFER, length, data, GL_STATIC_DRAW);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glEnableVertexAttribArray(attribute);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glVertexAttribPointer(attribute, element_size, GL_FLOAT, GL_FALSE, 0, 0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	doUnbindCheck();
	buffer_objects.push_back(vbo);
}

void VertexArrayObject::bindIndexArrayData(GLuint * data, size_t length)
{
	this->num_indices = length / sizeof(GLuint);
	GLuint vbo;
	doBindCheck();
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, length, data, GL_STATIC_DRAW);
	doUnbindCheck();
	buffer_objects.push_back(vbo);
}

void VertexArrayObject::render(void)
{
	doBindCheck();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	doUnbindCheck();
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

void VertexArrayObject::doBindCheck(void)
{
	if (currently_bound != vao_id)
	{
		glBindVertexArray(0);
		ErrorHandling::testGlError(__FILE__, __LINE__);
		glBindVertexArray(vao_id);
		ErrorHandling::testGlError(__FILE__, __LINE__);
	}
}

void VertexArrayObject::doUnbindCheck(void)
{
	if (currently_bound != vao_id)
	{
		glBindVertexArray(0);
		ErrorHandling::testGlError(__FILE__, __LINE__);
		glBindVertexArray(currently_bound);
		ErrorHandling::testGlError(__FILE__, __LINE__);
	}
}
