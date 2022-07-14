#include "ShadedEnt.h"

ShadedEnt::ShadedEnt(VertexArrayObject * vao, Shader * shader)
: vao(vao), shader(shader)
{
}

void ShadedEnt::draw(Matrix4& cam_inv, Matrix4& P, Vector4& clip)
{
	shader->bind();
	vao->bind();
	glUniformMatrix4fv(shader->getUniformLocation("cam_inv"), 1, GL_FALSE, cam_inv.ptr());
	glUniformMatrix4fv(shader->getUniformLocation("proj"), 1, GL_FALSE, P.ptr());
	glUniform4fv(shader->getUniformLocation("clip"), 1, clip.ptr());
	vao->render();
	vao->unbind();
	shader->unbind();
}
