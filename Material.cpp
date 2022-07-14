#include "Material.h"

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

Material::Material()
{
    //Material setup goes hurr
    ambientColor = Color::ambientMaterialDefault();
    diffuseColor = Color::diffuseMaterialDefault();
    specularColor = Color::specularMaterialDefault();
    emissionColor = Color::emissionMaterialDefault();
    shininess = 10.0;
    
    //Set the initial color to a random pastel color
    //Can be removed once materials are implemented
    color = Color::randomPastel();
}

Material::~Material()
{
    //Delete any dynamically allocated memory/objects here
}

void Material::apply(void)
{
    //Set the material properties for ambient, diffuse, specular, emission, and shininess
    //Hint: Lookup how glMaterialfv works
    
    //Set the color property (can be removed once materials are implemented)
    //Used for projects 1, 2, and 3
    //glColor3fv(color.ptr());
	glMaterialf(GL_FRONT, GL_SHININESS, shininess);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor.ptr());
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor.ptr());
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor.ptr());
	glMaterialfv(GL_FRONT, GL_EMISSION, emissionColor.ptr());
}

void Material::setAmbient(float x, float y, float z)
{
	ambientColor[0] = x;
	ambientColor[1] = y;
	ambientColor[2] = z;
	ambientColor[3] = 1.0;
}

void Material::setDiffuse(float x, float y, float z)
{
	diffuseColor[0] = x;
	diffuseColor[1] = y;
	diffuseColor[2] = z;
	diffuseColor[3] = 1.0;
}

void Material::setSpecular(float x, float y, float z)
{
	specularColor[0] = x;
	specularColor[1] = y;
	specularColor[2] = z;
	specularColor[3] = 1.0;
}

void Material::setEmission(float x, float y, float z)
{
	emissionColor[0] = x;
	emissionColor[1] = y;
	emissionColor[2] = z;
	emissionColor[3] = 1.0;
}

void Material::setTextureName(std::string textName)
{
	textureName += textName;
}

std::string Material::getTextureName()
{
	return textureName;
}

void Material::loadTexture()
{
	picture = Texture(textureName.c_str());
}

void Material::setMaterialName(std::string matName)
{
	materialName += matName;
}

std::string Material::getMaterialName()
{
	return materialName;
}
