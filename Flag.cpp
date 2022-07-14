#include "Flag.h"
#include <GL/GLee.h>
#include <GL/freeglut.h>

#include <vector>
#include <iostream>
#include <cstring>
#include "ErrorHandling.h"

#define INTERPOLATE_AMT 50
#define INTERP_INC (INTERPOLATE_AMT + 1)

Flag::Flag(BezierPatch s, Texture tex) : flag_surface(s), tex(tex)
{
	static const char * vert_shader_code =
		"#version 120\nvarying vec3 normal;\n void main() {\n gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;\n normal = gl_NormalMatrix * gl_Normal; gl_TexCoord[0] = gl_MultiTexCoord0;\n}";
	static const char * frag_shader_code =
		"#version 120\nvarying vec3 normal;\n uniform sampler2D tex;\n void main() {\n gl_FragColor.a = texture2D(tex, gl_TexCoord[0].st).r;\ngl_FragColor.r = normal.r; gl_FragColor.g = normal.g; gl_FragColor.b = normal.b;\n}";
	prog = glCreateProgram();
	GLint len = strlen(vert_shader_code);
	vert_shader = glCreateShader(GL_VERTEX_SHADER);
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vert_shader, 1, &vert_shader_code, &len);
	ErrorHandling::testGlError(__LINE__);
	len = strlen(frag_shader_code);
	glShaderSource(frag_shader, 1, &frag_shader_code, &len);
	ErrorHandling::testGlError(__LINE__);
	glAttachShader(prog, vert_shader);
	ErrorHandling::testGlError(__LINE__);
	glAttachShader(prog, frag_shader);
	ErrorHandling::testGlError(__LINE__);
	glCompileShader(frag_shader);
	GLsizei len_out = 0;
	std::vector<GLchar> infoLog(2500);
	ErrorHandling::testGlError(__LINE__);
	ErrorHandling::testGlError(__LINE__);
	glGetShaderInfoLog(vert_shader, 2500, &len_out, &infoLog[0]);
	if (len_out) std::cout << __LINE__  << ": " << &infoLog[0] << std::endl;
	glGetShaderInfoLog(frag_shader, 2500, &len_out, &infoLog[0]);
	if (len_out) std::cout << __LINE__  << ": " << &infoLog[0] << std::endl;
	ErrorHandling::testGlError(__LINE__);
	glLinkProgram(prog);
	ErrorHandling::testGlError(__LINE__);
	GLint out;
	glGetProgramiv(prog, GL_LINK_STATUS, &out);
	if (out == GL_FALSE)
	{
		std::cout << "link error" << std::endl;
		glGetProgramInfoLog(prog, 2500, &len_out, &infoLog[0]);
		if (len_out) std::cout << &infoLog[0] << std::endl;
	}
	ErrorHandling::testGlError(__LINE__);
}

void Flag::draw(void)
{
	GLfloat ps[INTERP_INC*INTERP_INC * 3];
	GLfloat ns[INTERP_INC*INTERP_INC * 3];
	GLuint indices[INTERPOLATE_AMT * INTERPOLATE_AMT * 4];
	GLfloat texture_coords[INTERP_INC * INTERP_INC * 2];
	for (int v = 0; v <= INTERPOLATE_AMT; ++v)
	{
		for (int u = 0; u <= INTERPOLATE_AMT; ++u)
		{
			float fu = (float)(u) / INTERPOLATE_AMT;
			float fv = (float)(v) / INTERPOLATE_AMT;
			if (fu < 0 || fu > 1)
			{
				std::cout << "danger fu" << std::endl;
			}
			if (fv < 0 || fv > 1)
			{
				std::cout << "danger fv" << std::endl;
			}
			Vector3 interp = flag_surface.interpolate(fu, fv);
			Vector3 norm = flag_surface.interpolateNormal(fu, fv).scale(-1).normalize();
			ps[v * INTERP_INC * 3 + u * 3] = interp[0];
			ps[v * INTERP_INC * 3 + u * 3 + 1] = interp[1];
			ps[v * INTERP_INC * 3 + u * 3 + 2] = interp[2];

			ns[v * INTERP_INC * 3 + u * 3] = norm[0];
			ns[v * INTERP_INC * 3 + u * 3 + 1] = norm[1];
			ns[v * INTERP_INC * 3 + u * 3 + 2] = norm[2];

			texture_coords[v * INTERP_INC * 2 + u * 2] = fu;
			texture_coords[v * INTERP_INC * 2 + u * 2 + 1] = 1.0f - fv;
		}
	}
	for (int row = 0; row < INTERPOLATE_AMT; ++row)
	{
		for (int column = 0; column < INTERPOLATE_AMT; ++column)
		{
			indices[row * INTERPOLATE_AMT * 4 + column * 4] = (row + 1) * INTERP_INC + column;
			indices[row * INTERPOLATE_AMT * 4 + column * 4 + 1] = (row + 1) * INTERP_INC + column + 1;
			indices[row * INTERPOLATE_AMT * 4 + column * 4 + 2] = row * INTERP_INC + column + 1;
			indices[row * INTERPOLATE_AMT * 4 + column * 4 + 3] = row * INTERP_INC + column;
		}
	}
	ErrorHandling::testGlError(__LINE__);
	glUseProgram(prog);
	ErrorHandling::testGlError(__LINE__);
	tex.bind();
	ErrorHandling::testGlError(__LINE__);
	glUniform1i(glGetUniformLocation(prog, "tex"), 0);
	ErrorHandling::testGlError(__LINE__);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, ps);
	glNormalPointer(GL_FLOAT, 0, ns);
	glTexCoordPointer(2, GL_FLOAT, 0, texture_coords);
	glDrawElements(GL_QUADS, INTERPOLATE_AMT * INTERPOLATE_AMT * 4, GL_UNSIGNED_INT, indices);
	tex.unbind();
	glUseProgram(0);
	ErrorHandling::testGlError(__LINE__);
}

