#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/freeglut.h>

#define IMAGE_TEXTURE_MAX 10

class FrameBuffer
{
private:
	// ID of the framebuffer. Assigned by OpenGL.
	GLuint buffer_id;
	// ID of the texture attached to the image buffer. Assigned by OpenGL.
	GLuint texture_id[IMAGE_TEXTURE_MAX];
	// ID of the texture attached to the depth buffer. Assigned by OpenGL.
	GLuint depth_texture_id;
	// ID of the non-texture depth buffer
	GLuint depth_buffer_id;

public:
	FrameBuffer(void);
	~FrameBuffer(void);

	// Creates a texture and attaches it to the framebuffer as an image buffer
	void createTextureAttachment(int, int);
	// Creates a texture and attaches it to the framebuffer at the given location
	void createTextureAttachment(int, int, int);
	void deleteTextureAttachment(void);
	void deleteTextureAttachment(int);
	void deleteDepthBufferAttachment(void);
	// Creates a texture and attaches it to the framebuffer as a depth buffer
	void createDepthTextureAttachment(int, int);
	// Creates a depth buffer and attaches it to the fraembuffer; not really sure why this is different from texture
	void createDepthBufferAttachment(int, int);
	// Instructs OpenGL to render to this buffer
	void bind(void);
	// Gets the id for this FrameBuffer's image texture
	int getImageTexture(void);
	int getImageTexture(int);
	void init(void);
	void bindImageTexture(void);
	void bindImageTexture(int);

	// Binds the default framebuffer
	static void bindDefaultBuffer(void);

	GLuint getId(void);
};

#endif // FRAMEBUFFER_H
