#ifndef VERTEXARRAYOBJECT_H
#define VERTEXARRAYOBJECT_H

#include <GL/freeglut.h>
#include <vector>

class VertexArrayObject
{
private:
	GLuint vao_id;
	static GLuint currently_bound;
	std::vector<GLuint> buffer_objects;
	size_t num_indices;

	void doBindCheck(void);
	void doUnbindCheck(void);

public:
	VertexArrayObject(void);
	~VertexArrayObject(void);
	void bind(void);
	void unbind(void);
	void enableVertexAttribArray(GLuint);

	void bindDataToAttribute(GLuint, GLint, float *, size_t);
	void bindIndexArrayData(GLuint *, size_t);

	void render(void);
};

#endif // VERTEXARRAYOBJECT_H
