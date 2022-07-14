#include "Globals.h"

Camera Globals::camera;

Light Globals::direction_light(Vector4(0.0f, 0.0f, 0.0f, 0.0f), false);

DrawData Globals::drawData;
UpdateData Globals::updateData;
ParticleSystem * Globals::system;

Texture Globals::texbox_back;
Texture Globals::texbox_base;
Texture Globals::texbox_front;
Texture Globals::texbox_left;
Texture Globals::texbox_right;
Texture Globals::texbox_top;
Texture Globals::tex_flag;
SkyBox * Globals::skybox;
bool Globals::use_shader(true);
OBJObjectTexture * Globals::whale;
OBJObjectTexture * Globals::boat;

void Globals::checkError(int line)
{
	GLenum error = glGetError();
	if (error != 0)
	{
		std::cout << __LINE__ << ": " << error << std::endl;
	}
}