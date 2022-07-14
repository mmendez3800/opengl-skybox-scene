#ifndef SHADER_H
#define SHADER_H

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
    #include <OpenGL/glext.h>
#else
    #include <GL/GLee.h>
#endif

/*! Handles GLSL shaders.  It can load the code from a file or read straight
 * from a char array. */
class Shader
{
    
private:
    
	char* read(const char *filename);
	void setup(const char *vs, const char *fs);

protected:
    
	GLuint pid;
    static GLuint currentlyBoundShaderID;
    
public:
    
	Shader(const char *vert, const char *frag, bool isFile=true);
	~Shader();
    
	void bind();
	void unbind();
    
	GLuint getPid();
    
	void printLog(const char* tag = "");

	GLint getUniformLocation(const GLchar *);
	GLint getAttribLocation(const GLchar *);
};

#endif
