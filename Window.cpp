#include <iostream>

#include "Window.h"
#include "SingleQuad.h"
#include "Globals.h"
#include "Matrix4.h"
#include "Vector4.h"
#include <algorithm>
#include "ErrorHandling.h"
#include "VBOQuad.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define Z_VAL 1
#define HALFWIDTH 2.0f
#define WATER_HEIGHT -2.0f
#define WATER_HALFWIDTH 401.0f

Matrix4 Window::D;
Matrix4 Window::P;

int Window::width = 512;   //Set window width in pixels here
int Window::height = 512;   //Set window height in pixels here

bool Window::show_panes(false);
bool Window::bloom_panes(false);
bool Window::toggle_bloom(true);

int Window::mouse_button;
int Window::mouse_drag_prev_x;
int Window::mouse_drag_prev_y;
Vector3 Window::mouse_drag_prev_vec;
FrameBuffer Window::refract_buffer;
FrameBuffer Window::reflect_buffer;
FrameBuffer Window::bloom_buffer_a;
FrameBuffer Window::bloom_buffer_b;
FrameBuffer Window::img_buffer;
Camera Window::reflect_cam;

VBOQuad * Window::quad;
VBOQuad * Window::quad_v;
WaveQuad * Window::wave_quad;

Shader * Window::bloom_filter;
Shader * Window::gauss_horiz;
Shader * Window::gauss_vert;
Shader * Window::bloom_recomb;

Matrix4 Window::boat_transform;
Matrix4 Window::whale_transform;

void Window::initialize(void)
{
	//Setup the light
	Globals::direction_light.position = Vector3(0.0f, -0.1f, 1.0f).normalize().toVector4(0.0f);
	Globals::direction_light.bind(0, Globals::camera);

	Globals::camera.set(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	Globals::camera.update();
	Globals::texbox_back = Texture("sb_back.ppm");
	Globals::texbox_base = Texture("sb_base.ppm");
	Globals::texbox_front = Texture("sb_front.ppm");
	Globals::texbox_left = Texture("sb_left.ppm");
	Globals::texbox_right = Texture("sb_right.ppm");
	Globals::texbox_top = Texture("sb_top.ppm");
	Globals::tex_flag = Texture("Ucsd-logo.ppm");
	Globals::skybox = new SkyBox(Globals::texbox_front, Globals::texbox_back, Globals::texbox_top, Globals::texbox_base, Globals::texbox_left, Globals::texbox_right);

	Globals::system = new ParticleSystem(0, Vector3(5.8f, 0.25f, 0));

	refract_buffer.init();
	
	reflect_buffer.init();

	img_buffer.init();

	bloom_buffer_a.init();
	bloom_buffer_b.init();

	prepareBuffers();

	ErrorHandling::testGlError(__FILE__, __LINE__);

	quad = new VBOQuad(Vector3(-10, WATER_HEIGHT, -10), Vector3(-10, WATER_HEIGHT, 10), Vector3(10, WATER_HEIGHT, 10), Vector3(10, WATER_HEIGHT, -10));
	quad_v = new VBOQuad(Vector3(-HALFWIDTH, HALFWIDTH, Z_VAL), Vector3(-HALFWIDTH, -HALFWIDTH, Z_VAL), Vector3(HALFWIDTH, -HALFWIDTH, Z_VAL), Vector3(HALFWIDTH, HALFWIDTH, Z_VAL));
	Globals::camera.cloneInto(reflect_cam);
	reflect_cam.e[1] = -reflect_cam.e[1] + 2*WATER_HEIGHT;
	reflect_cam.d[1] = -reflect_cam.d[1] + 2*WATER_HEIGHT;
	reflect_cam.update();

	wave_quad = new WaveQuad(Vector3(-WATER_HALFWIDTH, WATER_HEIGHT, -WATER_HALFWIDTH), Vector3(-WATER_HALFWIDTH, WATER_HEIGHT, WATER_HALFWIDTH), Vector3(WATER_HALFWIDTH, WATER_HEIGHT, WATER_HALFWIDTH), Vector3(WATER_HALFWIDTH, WATER_HEIGHT, -WATER_HALFWIDTH), reflect_buffer.getImageTexture(), refract_buffer.getImageTexture());

	bloom_filter = new Shader("bloom_filter.vert", "bloom_filter.frag", true);
	gauss_horiz = new Shader("bloom_filter.vert", "gauss_filter_horiz.frag", true);
	gauss_vert = new Shader("bloom_filter.vert", "gauss_filter_vert.frag", true);
	bloom_recomb = new Shader("bloom_filter.vert", "bloom_combine.frag", true);

	Globals::whale = new OBJObjectTexture("Oshus (Whale).obj", 2);
	Globals::boat = new OBJObjectTexture("Going Merry.obj", 3);

	for (unsigned int i = 0; i < Globals::whale->materials.size(); i++) {
		Globals::whale->materials[i].loadTexture();
	}
	for (unsigned int i = 0; i < Globals::boat->materials.size(); i++) {
		Globals::boat->materials[i].loadTexture();
	}

	whale_transform.identity();
	boat_transform.identity();

	Matrix4 working;
	working.makeRotateY(M_PI_2);
	boat_transform = boat_transform * working;
	working.makeScale(0.005f);
	boat_transform = boat_transform * working;
	working.makeTranslate(0, -1.2f, 0);
	boat_transform = working * boat_transform;
}

void Window::prepareBuffers(void)
{
	refract_buffer.createTextureAttachment(width, height);
	refract_buffer.createDepthBufferAttachment(width, height);

	reflect_buffer.createTextureAttachment(width, height);
	reflect_buffer.createDepthBufferAttachment(width, height);

	img_buffer.createTextureAttachment(width, height);
	img_buffer.createDepthBufferAttachment(width, height);

	bloom_buffer_a.createTextureAttachment(0, width, height);
	bloom_buffer_b.createTextureAttachment(0, width, height);
}

void Window::recycleBuffers(void)
{
	refract_buffer.deleteTextureAttachment();
	refract_buffer.deleteDepthBufferAttachment();

	reflect_buffer.deleteTextureAttachment();
	reflect_buffer.deleteDepthBufferAttachment();

	img_buffer.deleteTextureAttachment();
	img_buffer.deleteDepthBufferAttachment();

	bloom_buffer_a.deleteTextureAttachment(0);
	bloom_buffer_b.deleteTextureAttachment(0);
}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
// This is called at the start of every new "frame" (qualitatively)
void Window::idleCallback()
{
	static int time = 0;
	static int prev_time = 0;
	static int frames = 0;

	++frames;
	//Set up a static time delta for update calls
	Globals::updateData.dt = 1.0 / 60.0;// 60 fps

	//Call the display routine to draw the cube
	displayCallback();

	// FRAMERATE STUFF
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - prev_time > 1000)
	{
		std::cout << "Framerate: " << (frames * 1000.0 / (time - prev_time)) << std::endl;
		frames = 0;
		prev_time = time;
	}
	Globals::skybox->tick();
	wave_quad->tick();

	Globals::system->update(Globals::updateData);
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
	width = w;                                                       //Set the window width
	height = h;                                                      //Set the window height
	glViewport(0, 0, w, h);                                          //Set new viewport size
	glMatrixMode(GL_PROJECTION);                                     //Set the OpenGL matrix mode to Projection
	glLoadIdentity();                                                //Clear the projection matrix by loading the identity
	gluPerspective(60.0f, float(width) / (float)height, 1.0f, 1000.0f); //Set perspective projection viewing frustum
	D.makeViewport(0, (float) width, 0, (float) height);
	P.makePerspectiveProjection(60.0f, (float) width, (float) height, 1.0f, 1000.0f);
	glLoadMatrixf(P.ptr());

	recycleBuffers();
	prepareBuffers();
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
	ErrorHandling::testGlError(__FILE__, __LINE__);

	renderRefract();
	renderReflect();

	//Clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set the OpenGL matrix mode to ModelView
	glMatrixMode(GL_MODELVIEW);
	//Push a matrix save point
	//This will save a copy of the current matrix so that we can
	//make changes to it and 'pop' those changes off later.
	glPushMatrix();

	//Replace the current top of the matrix stack with the inverse camera matrix
	//This will convert all world coordiantes into camera coordiantes
	glLoadMatrixf(Globals::camera.getInverseMatrix().ptr());

	Vector4 clip(0.0f, 0.0f, 0.0f, 1.0f); // no clipping

	if (toggle_bloom)
	{
		img_buffer.bind();
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glMultMatrixf(boat_transform.ptr());
	Globals::boat->draw(Globals::camera.getInverseMatrix(), P, boat_transform, clip);
	glPopMatrix();
	Globals::skybox->draw(Globals::camera.getInverseMatrix(), P, clip);
	wave_quad->draw(Globals::camera, P, clip);
	//quad_v->draw(Globals::camera.getInverseMatrix(), P, clip);
	Globals::system->draw(Globals::camera, P, clip);
	ErrorHandling::testGlError(__FILE__, __LINE__);

	if (toggle_bloom)
	{
		img_buffer.bindDefaultBuffer();

		shaderMuddle(img_buffer.getImageTexture(), bloom_buffer_a.getId(), *bloom_filter);
		shaderMuddle(bloom_buffer_a.getImageTexture(), bloom_buffer_b.getId(), *gauss_horiz);
		shaderMuddle(bloom_buffer_b.getImageTexture(), bloom_buffer_a.getId(), *gauss_vert);
		shaderMuddle(bloom_buffer_a.getImageTexture(), bloom_buffer_b.getId(), *gauss_horiz);
		shaderMuddle(bloom_buffer_b.getImageTexture(), bloom_buffer_a.getId(), *gauss_vert);
		shaderMuddle(bloom_buffer_a.getImageTexture(), bloom_buffer_b.getId(), *gauss_horiz);
		shaderMuddle(bloom_buffer_b.getImageTexture(), bloom_buffer_a.getId(), *gauss_vert);
		shaderMuddle(bloom_buffer_a.getImageTexture(), bloom_buffer_b.getId(), *gauss_horiz);
		shaderMuddle(bloom_buffer_b.getImageTexture(), bloom_buffer_a.getId(), *gauss_vert);
		shaderMuddle(bloom_buffer_a.getImageTexture(), bloom_buffer_b.getId(), *gauss_horiz);
		shaderMuddle(bloom_buffer_b.getImageTexture(), bloom_buffer_a.getId(), *gauss_vert);
		shaderMuddle(bloom_buffer_a.getImageTexture(), bloom_buffer_b.getId(), *gauss_horiz);
		shaderMuddle(bloom_buffer_b.getImageTexture(), bloom_buffer_a.getId(), *gauss_vert);
		shaderMuddle(bloom_buffer_a.getImageTexture(), bloom_buffer_b.getId(), *gauss_horiz);
		shaderMuddle(bloom_buffer_b.getImageTexture(), bloom_buffer_a.getId(), *gauss_vert);
		shaderMuddle(bloom_buffer_a.getImageTexture(), bloom_buffer_b.getId(), *gauss_horiz);
		shaderMuddle(bloom_buffer_b.getImageTexture(), bloom_buffer_a.getId(), *gauss_vert);
		shaderMuddle(bloom_buffer_a.getImageTexture(), bloom_buffer_b.getId(), *gauss_horiz);
		shaderMuddle(bloom_buffer_b.getImageTexture(), bloom_buffer_a.getId(), *gauss_vert);

		bloomRecombine();
		if (bloom_panes)
		{
			renderPanes(img_buffer, bloom_buffer_a);
		}
	}

	//Pop off the changes we made to the matrix stack this frame
	glPopMatrix();
	ErrorHandling::testGlError(__FILE__, __LINE__);

	// Important that this happens last
	if (show_panes) renderPanes();
	ErrorHandling::testGlError(__FILE__, __LINE__);

	//Tell OpenGL to clear any outstanding commands in its command buffer
	//This will make sure that all of our commands are fully executed before
	//we swap buffers and show the user the freshly drawn frame
	glFlush();
	ErrorHandling::testGlError(__FILE__, __LINE__);

	//Swap the off-screen buffer (the one we just drew to) with the on-screen buffer
	glutSwapBuffers();
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

void Window::shaderMuddle(GLuint sourceTex, GLuint destBuf, Shader& shader)
{
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glActiveTexture(GL_TEXTURE0);
	glBindFramebuffer(GL_FRAMEBUFFER, destBuf);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, sourceTex);
	shader.bind();
	glUniform1f(0, 1.0f / width);
	glUniform1f(1, 1.0f / height);
	glUniform1i(2, 0);
	fullQuad();
	shader.unbind();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

void Window::bloomRecombine(void)
{
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glActiveTexture(GL_TEXTURE0);
	img_buffer.bindImageTexture();
	glActiveTexture(GL_TEXTURE1);
	bloom_buffer_a.bindImageTexture();

	bloom_recomb->bind();

	glUniform1f(0, 1.0f / width);
	glUniform1f(1, 1.0f / height);
	//glUniform1i(bloom_recomb->getUniformLocation("base_img"), 0);
	//glUniform1i(bloom_recomb->getUniformLocation("bloom_img"), 1);
	glUniform1i(10, 0);
	glUniform1i(11, 1);

	fullQuad();

	bloom_recomb->unbind();

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

void Window::fullQuad(void)
{
	Matrix4 orth;
	// Put us into a mode where our quad will fill the screen
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glMatrixMode(GL_PROJECTION);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glPushMatrix();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glLoadIdentity();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	//glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT), -1.0, 1.0);
	//glOrtho(0.0, 5, 0.0, 5, -1.0, 1.0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glMatrixMode(GL_MODELVIEW);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glPushMatrix();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glLoadIdentity();
	ErrorHandling::testGlError(__FILE__, __LINE__);

	glDisable(GL_DEPTH_TEST);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	orth.makeOrtho(0, 1, 0, 1, -1, 1);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glUniformMatrix4fv(3, 1, GL_FALSE, orth.ptr());
	ErrorHandling::testGlError(__FILE__, __LINE__);

	//Quad rendering here...
	glBegin(GL_TRIANGLE_STRIP);
	glVertexAttrib2f(0, 0.0f, 0.0f);
	glVertex2d(0, 0); // this is basically ignored
	glVertexAttrib2f(0, 0.0f, 1.0f);
	glVertex2d(0, 0); // this is basically ignored
	glVertexAttrib2f(0, 1.0f, 1.0f);
	glVertex2d(0, 0); // this is basically ignored
	glVertexAttrib2f(0, 1.0f, 0.0f);
	glVertex2d(0, 0); // this is basically ignored
	glEnd();
	ErrorHandling::testGlError(__FILE__, __LINE__);

	glPopMatrix();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glMatrixMode(GL_PROJECTION);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glPopMatrix();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glMatrixMode(GL_MODELVIEW);
	ErrorHandling::testGlError(__FILE__, __LINE__);

	glEnable(GL_DEPTH_TEST);
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

void Window::renderPanes(void)
{
	renderPanes(reflect_buffer, refract_buffer);
}

void Window::renderPanes(FrameBuffer& left, FrameBuffer& right)
{
	// render any framebuffers necessary
	glMatrixMode(GL_PROJECTION);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glPushMatrix();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glLoadIdentity();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	// The big number is important to make sure that this gets put at the front of everything
	glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), 0.0, glutGet(GLUT_WINDOW_HEIGHT), -1.0, 1.0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glMatrixMode(GL_MODELVIEW);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glPushMatrix();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glLoadIdentity();
	ErrorHandling::testGlError(__FILE__, __LINE__);

	glDisable(GL_DEPTH_TEST);
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	static GLfloat quad_verts[] = { 0, 0, 0, 0, height / 8.0f, 0, width / 8.0f, height / 8.0f, 0, width / 8.0f, 0, 0 };
	static GLfloat quad_tex_verts[] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f };
	static GLuint quad_inds[] = { 0, 1, 2, 3 };
	glDisable(GL_COLOR_MATERIAL);
	//glDisable(GL_LIGHTING);
	glColor3f(1,1,1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, quad_verts);
	glTexCoordPointer(2, GL_FLOAT, 0, quad_tex_verts);
	//glBindTexture(GL_TEXTURE_2D, refract_buffer.getImageTexture());
	left.bindImageTexture();
	//glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, quad_inds);
	// Draw a textured quad
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0, 0, 0);
	glTexCoord2f(0, 1); glVertex3f(0, height / 2.0f, 0);
	glTexCoord2f(1, 1); glVertex3f(width / 2.0f, height / 2.0f, 0);
	glTexCoord2f(1, 0); glVertex3f(width / 2.0f, 0, 0);
	glEnd();
	right.bindImageTexture();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(width / 2.0f, 0, 0);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(width / 2.0f, height / 2.0f, 0);
	glTexCoord2f(1.0f, 1.0f); glVertex3f((float) width, height / 2.0f, 0);
	glTexCoord2f(1.0f, 0.0f); glVertex3f((float) width, 0, 0);
	glEnd();
	Globals::texbox_top.unbind();
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();
	ErrorHandling::testGlError(__FILE__, __LINE__);

	glMatrixMode(GL_PROJECTION);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glPopMatrix();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glMatrixMode(GL_MODELVIEW);
	ErrorHandling::testGlError(__FILE__, __LINE__);

	//glEnable(GL_LIGHTING);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glEnable(GL_DEPTH_TEST);
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

void Window::renderRefract(void)
{
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glEnable(GL_CLIP_DISTANCE0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	refract_buffer.bind();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	//Clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ErrorHandling::testGlError(__FILE__, __LINE__);

	//Set the OpenGL matrix mode to ModelView
	glMatrixMode(GL_MODELVIEW);
	//Push a matrix save point
	//This will save a copy of the current matrix so that we can
	//make changes to it and 'pop' those changes off later.
	glPushMatrix();

	//Replace the current top of the matrix stack with the inverse camera matrix
	//This will convert all world coordiantes into camera coordiantes
	glLoadMatrixf(Globals::camera.getInverseMatrix().ptr());
	ErrorHandling::testGlError(__FILE__, __LINE__);

	Vector4 clip(0, -1, 0, WATER_HEIGHT);

	Globals::skybox->draw(Globals::camera.getInverseMatrix(), P, clip);
	glPushMatrix();
	glMultMatrixf(boat_transform.ptr());
	Globals::boat->draw(Globals::camera.getInverseMatrix(), P, boat_transform, clip);
	glPopMatrix();
	//quad_v->draw(Globals::camera.getInverseMatrix(), P, clip);
	Globals::system->draw(Globals::camera, P, clip);
	ErrorHandling::testGlError(__FILE__, __LINE__);

	//Pop off the changes we made to the matrix stack this frame
	glPopMatrix();

	//Tell OpenGL to clear any outstanding commands in its command buffer
	//This will make sure that all of our commands are fully executed before
	//we swap buffers and show the user the freshly drawn frame
	glFlush();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	FrameBuffer::bindDefaultBuffer();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glDisable(GL_CLIP_DISTANCE0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
}

void Window::renderReflect(void)
{
	//Vector3 up_save = reflect_cam.up;
	//reflect_cam.up = up_save.scale(-1);
	//reflect_cam.update();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glEnable(GL_CLIP_DISTANCE0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	reflect_buffer.bind();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	//Clear color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ErrorHandling::testGlError(__FILE__, __LINE__);

	//Set the OpenGL matrix mode to ModelView
	glMatrixMode(GL_MODELVIEW);
	//Push a matrix save point
	//This will save a copy of the current matrix so that we can
	//make changes to it and 'pop' those changes off later.
	glPushMatrix();

	//Replace the current top of the matrix stack with the inverse camera matrix
	//This will convert all world coordiantes into camera coordiantes
	glLoadMatrixf(reflect_cam.getInverseMatrix().ptr());
	ErrorHandling::testGlError(__FILE__, __LINE__);

	Vector4 clip(0, 1, 0, -WATER_HEIGHT);

	Globals::skybox->draw(reflect_cam.getInverseMatrix(), P, clip);
	glPushMatrix();
	glMultMatrixf(boat_transform.ptr());
	Globals::boat->draw(reflect_cam.getInverseMatrix(), P, boat_transform, clip);
	glPopMatrix();
	//quad_v->draw(reflect_cam.getInverseMatrix(), P, clip);
	Globals::system->draw(reflect_cam, P, clip);
	ErrorHandling::testGlError(__FILE__, __LINE__);

	//Pop off the changes we made to the matrix stack this frame
	glPopMatrix();

	//Tell OpenGL to clear any outstanding commands in its command buffer
	//This will make sure that all of our commands are fully executed before
	//we swap buffers and show the user the freshly drawn frame
	glFlush();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	FrameBuffer::bindDefaultBuffer();
	ErrorHandling::testGlError(__FILE__, __LINE__);
	glDisable(GL_CLIP_DISTANCE0);
	ErrorHandling::testGlError(__FILE__, __LINE__);
	//reflect_cam.up = up_save;
	//reflect_cam.update();
}

void Window::mousePressCallback(int button, int state, int x, int y)
{
	mouse_button = button;
	mouse_drag_prev_x = x;
	mouse_drag_prev_y = x;
	mouse_drag_prev_vec = trackBallMapping(x, y);
	if (state == GLUT_UP)
	{
		mouse_button = -1;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
	}
}

void Window::mouseDragCallback(int x, int y)
{
	Matrix4 transform;
	if (mouse_button == GLUT_LEFT_BUTTON)
	{
		transform = calculateMouseDragRotation(x, y);
		Globals::camera.e = transform * Globals::camera.e;
		Globals::camera.update();
		Globals::camera.cloneInto(reflect_cam);
		reflect_cam.e[1] = -reflect_cam.e[1] + 2*WATER_HEIGHT;
		reflect_cam.d[1] = -reflect_cam.d[1] + 2*WATER_HEIGHT;
		reflect_cam.update();
	}

	mouse_drag_prev_x = x;
	mouse_drag_prev_y = y;
}

Matrix4 Window::calculateMouseDragRotation(int x, int y)
{
	Matrix4 transform;
	Vector3 n_vec = trackBallMapping(x, y);
	Vector3 cross = mouse_drag_prev_vec.cross(n_vec);
	float dot = mouse_drag_prev_vec.dot(n_vec);
	if (dot != dot)
	{
		transform.identity();
		return transform;
	}
	float angle = acos(mouse_drag_prev_vec.dot(n_vec)); // Magnitude not needed because normalized
	if (angle != angle)
	{
		transform.identity();
		return transform;
	}
	mouse_drag_prev_vec = n_vec;
	transform.makeRotateArbitrary(cross, angle);
	return transform;
}

Vector3 Window::trackBallMapping(int x, int y)    // The CPoint class is a specific Windows class. Either use separate x and y values for the mouse location, or use a Vector3 in which you ignore the z coordinate.
{
	Vector3 v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
	float d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
	v[0] = (2.0f*x - width) / width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
	v[1] = (height - 2.0f*y) / height;   // this does the equivalent to the above for the mouse Y position
	v[2] = 0.0f;   // initially the mouse z position is set to zero, but this will change below
	d = v.magnitude();    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
	d = (d < 1.0f) ? d : 1.0f;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
	v[2] = sqrtf(1.001f - d*d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
	v = v.normalize(); // Still need to normalize, since we only capped d, not v.
	return v;  // return the mouse location on the surface of the trackball
}

void Window::keyboardCallback(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'p':
		show_panes = !show_panes;
		bloom_panes = false;
		break;
	case 't':
		toggle_bloom = !toggle_bloom;
		break;
	case 'b':
		bloom_panes = !bloom_panes;
		show_panes = false;
		break;
	}
}
