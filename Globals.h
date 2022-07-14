#ifndef GLOBALS_H
#define GLOBALS_H

#include "SkyBox.h"
#include "Camera.h"
#include "Light.h"
#include "DrawData.h"
#include "UpdateData.h"
#include <GL/glut.h>
#include "Texture.h"
#include "ParticleSystem.h"
#include "OBJObjectTexture.h"

class Globals
{
    
public:
    
    static Camera camera;
    static Light direction_light;
    static DrawData drawData;
    static UpdateData updateData;
	static Texture texbox_back, texbox_base, texbox_front, texbox_left, texbox_right, texbox_top;
	static SkyBox * skybox;
	static Texture tex_flag;
	static bool use_shader;
    //Feel free to add more member variables as needed

	static ParticleSystem* system;
	static void checkError(int = __LINE__);

	static OBJObjectTexture * boat;
	static OBJObjectTexture * whale;
};

#endif
