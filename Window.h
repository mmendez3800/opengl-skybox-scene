#ifndef WINDOW_H
#define WINDOW_H
#define _USE_MATH_DEFINES
#include "VBOQuad.h"
#include <string>
#include <math.h>
#include "Matrix4.h"
#include <random>
#include "FrameBuffer.h"
#include "Camera.h"
#include "WaveQuad.h"


class Window	  // OpenGL output window related routines
{
private:
	static Matrix4 D;
	static Matrix4 P;
	static int mouse_button;
	static int mouse_drag_prev_x;
	static int mouse_drag_prev_y;
	static Vector3 mouse_drag_prev_vec;
	static FrameBuffer refract_buffer;
	static FrameBuffer reflect_buffer;
	static FrameBuffer img_buffer;
	static FrameBuffer bloom_buffer_a;
	static FrameBuffer bloom_buffer_b;
	static VBOQuad * quad;
	static VBOQuad * quad_v;
	static WaveQuad * wave_quad;
	static Camera reflect_cam;
	static bool show_panes;
	static bool toggle_bloom;
	static bool bloom_panes;
	static Shader * bloom_filter;
	static Shader * gauss_horiz;
	static Shader * gauss_vert;
	static Shader * bloom_recomb;

	static void prepareBuffers(void);
	static void recycleBuffers(void);
	static Matrix4 boat_transform;
	static Matrix4 whale_transform;

public:

	static int width, height; 	            // window size

	static void initialize(void);
	static void idleCallback(void);
	static void reshapeCallback(int, int);
	static void displayCallback(void);
	static void mousePressCallback(int, int, int, int);
	static void mouseDragCallback(int, int);
	static void keyboardCallback(unsigned char, int, int);
	static Matrix4 calculateMouseDragRotation(int, int);
	static Vector3 trackBallMapping(int, int);
	static void renderRefract(void);
	static void renderReflect(void);
	static void renderPanes(void);
	static void renderPanes(FrameBuffer&, FrameBuffer&);
	static void bloomFilter(void);
	static void fullQuad(void);
	static void bloomRecombine(void);
	static void shaderMuddle(GLuint, GLuint, Shader&);
};

#endif

