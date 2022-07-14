#include <GL/GLee.h>
#include "FrameBuffer.h"
#include <GL/freeglut.h>
#include "ErrorHandling.h"
#include <iostream>

FrameBuffer::FrameBuffer(void)
{
}

void FrameBuffer::init(void)
{
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glGenFramebuffers(1, &buffer_id);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer_id);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	// Not really sure what this does
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	// Clean up after ourselves, we aren't trying to bind anything here
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

FrameBuffer::~FrameBuffer(void)
{
	// Free up our GL space
	glDeleteFramebuffers(1, &buffer_id);
	// No need for any textures anymore
	for (int i = 0; i < IMAGE_TEXTURE_MAX; ++i)
	{
		if (texture_id)
		{
			glDeleteTextures(1, &texture_id[i]);
			texture_id[i] = 0;
		}
	}
	if (depth_texture_id)
	{
		glDeleteTextures(1, &depth_texture_id);
		depth_texture_id = 0;
	}
	if (depth_buffer_id)
	{
		glDeleteRenderbuffers(1, &depth_buffer_id);
		depth_buffer_id = 0;
	}
}

void FrameBuffer::createTextureAttachment(int width, int height)
{
	createTextureAttachment(0, width, height);
}

void FrameBuffer::deleteTextureAttachment(void)
{
	deleteTextureAttachment(0);
}

void FrameBuffer::deleteTextureAttachment(int location)
{
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + location, GL_TEXTURE_2D, texture_id[location], 0);
	glDeleteTextures(1, &texture_id[location]);
	texture_id[location] = 0;
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

void FrameBuffer::createTextureAttachment(int location, int width, int height)
{
	if (location >= IMAGE_TEXTURE_MAX)
	{
		std::cerr << "Improper texture location: " << location << std::endl;
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, buffer_id);
	glGenTextures(1, &texture_id[location]);
	if (texture_id == 0)
	{
		std::cerr << __FILE__ << ": " << __LINE__ << ": texid is zero" << std::endl;
	}
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glBindTexture(GL_TEXTURE_2D, texture_id[location]);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + location, GL_TEXTURE_2D, texture_id[location], 0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	// Don't leave any loose ends
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glBindTexture(GL_TEXTURE_2D, 0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

void FrameBuffer::createDepthTextureAttachment(int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, buffer_id);
	glGenTextures(1, &depth_texture_id);
	glBindTexture(GL_TEXTURE_2D, depth_texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_texture_id, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::createDepthBufferAttachment(int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, buffer_id);
	glGenRenderbuffers(1, &depth_buffer_id);
	glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_id);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_id);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void FrameBuffer::deleteDepthBufferAttachment(void)
{
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
	glDeleteRenderbuffers(1, &depth_buffer_id);
	depth_buffer_id = 0;
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

void FrameBuffer::bindDefaultBuffer(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bind(void)
{
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer_id);
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

void FrameBuffer::bindImageTexture(void)
{
	bindImageTexture(0);
}

void FrameBuffer::bindImageTexture(int location)
{
	glBindTexture(GL_TEXTURE_2D, texture_id[location]);
	// Make sure no bytes are padded:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Select GL_MODULATE to mix texture with polygon color for shading:
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Use bilinear interpolation:
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

int FrameBuffer::getImageTexture(void)
{
	return getImageTexture(0);
}

int FrameBuffer::getImageTexture(int location)
{
	return texture_id[location];
}

GLuint FrameBuffer::getId(void)
{
	return buffer_id;
}
