#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>

#include "Shader.h"
#include "ErrorHandling.h"

GLuint Shader::currentlyBoundShaderID = 0x0;

Shader::Shader(const char *vert, const char *frag, bool isFile)
{
	if(isFile)
	{
        //Read in the vertex and fragment shaders
        //We must delete these after we are finished compiling the shaders
		char* vv = read(vert);
		char* vf = read(frag);
        
        //Setup the shader
		setup(vv, vf);
        
        //Delete the file data arrays we allocted
		delete[] vv;
		delete[] vf;
	}
	else
    {
        //Treat the vert and frag parameters as shader code and directly compile them
		setup(vert, frag);
    }
}

Shader::~Shader()
{
	glDeleteProgram(pid);
}

void Shader::bind()
{
    if(currentlyBoundShaderID != pid)
    {
        currentlyBoundShaderID = pid;
        glUseProgram(pid);
		ErrorHandling::testGlError(__FILE__, __LINE__);
    }
}

void Shader::unbind()
{
    if(currentlyBoundShaderID != 0)
    {
        currentlyBoundShaderID = 0;
        glUseProgram(0);
    }
}

void Shader::printLog(const char* tag)
{
	char glslLog[1024];
	GLsizei glslLogSize = 0;
    
    //Extract the error log for this shader's pid
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glGetProgramInfoLog(pid, 1024, &glslLogSize, glslLog);
	ErrorHandling::testGlError(__FILE__, __LINE__);
    
    //If the log isn't empty, print the contents
	if(glslLogSize > 0)
        std::cerr << tag << "(" << pid << ") -  Shader error log:" << std::endl << glslLog << std::endl;
	else
        std::cerr << tag << "(" << pid << ") -  Shaders compiled successfully!" << std::endl;
}

char* Shader::read(const char *filename)
{
	char* shaderFile = 0;

	//Open the file
	FILE* fp = fopen(filename, "rb");
	if (!fp)
	{
		std::cerr << "File doesn't exist [" << filename << "]" << std::endl;
		std::exit(-1);
	}

	//Obtain the file size
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	rewind(fp);

	//Alloc memory - will be deleted while setting the shader up
	shaderFile = new char[size + 1];

	//Copy the file to the shaderFile
	fread(shaderFile, sizeof(char), size, fp);
	shaderFile[size] = '\0'; //Eliminate the garbage at EOF
	fclose(fp);

	return shaderFile;
}

void Shader::setup(const char *vs, const char *fs)
{
	//Create two new Shader Object IDs
	GLuint vid = glCreateShader(GL_VERTEX_SHADER);
	GLuint fid = glCreateShader(GL_FRAGMENT_SHADER);

	//Pass the shader source code to OpenGL
	glShaderSource(vid, 1, &vs, 0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glShaderSource(fid, 1, &fs, 0);
	ErrorHandling::testGlError(__FILE__, __LINE__);

	//Compile the shader files
	glCompileShader(vid);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glCompileShader(fid);
	ErrorHandling::testGlError(__FILE__, __LINE__);

	char glslLog[1024];
	GLsizei glslLogSize;

	//Get the error log for the Vertex shader
	glGetShaderInfoLog(vid, 1024, &glslLogSize, glslLog);
	if (glslLogSize)
	{
		//std::cerr << vs << ": " << std::endl;
		std::cerr << "Vertex program log: " << glslLog << std::endl;
	}

	//Get the error log for the Fragment shader
	glGetShaderInfoLog(fid, 1024, &glslLogSize, glslLog);
	if (glslLogSize)
	{
		//std::cerr << fs << ": " << std::endl;
		std::cerr << "Fragment program log: " << glslLog << std::endl;
	}

	//Create a new Shader Program
	pid = glCreateProgram();
	printf("Created shader %d\n", pid);
	ErrorHandling::testGlError(__FILE__, __LINE__);

	//Attach the Vertex and Fragment shaders to the Shader Program
	glAttachShader(pid, vid);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glAttachShader(pid, fid);
	ErrorHandling::testGlError(__FILE__, __LINE__);

	//Link it!
	glLinkProgram(pid);
	ErrorHandling::testGlError(__FILE__, __LINE__);

	// Get a few diagnostics
	GLint out;
	glGetProgramiv(pid, GL_LINK_STATUS, &out);
	if (out == GL_FALSE)
	{
		std::cout << "link error" << std::endl;
		GLint len_out;
		glGetProgramInfoLog(pid, 1024, &len_out, &glslLog[0]);
		if (len_out) std::cout << &glslLog[0] << std::endl;
	}
	glGetProgramiv(pid, GL_ATTACHED_SHADERS, &out);
	printf("Number shaders attached: %d\n", out);
	glGetProgramiv(pid, GL_ACTIVE_ATTRIBUTES, &out);
	printf("Number active attributes: %d\n", out);
	glGetProgramiv(pid, GL_ACTIVE_UNIFORMS, &out);
	printf("Number active uniforms: %d\n", out);

	//Delete shader objects since they have been attached to a program; no longer necessary
	glDeleteShader(vid);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glDeleteShader(fid);
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

GLint Shader::getUniformLocation(const GLchar * name)
{
	return glGetUniformLocation(pid, name);
}

GLuint Shader::getPid(void)
{
	return pid;
}

GLint Shader::getAttribLocation(const GLchar * name)
{
	return glGetAttribLocation(pid, name);
}

