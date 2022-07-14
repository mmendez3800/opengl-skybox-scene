#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include "Texture.h"
#include "Color.h"

class Material
{
    
public:
	Texture picture;
	std::string materialName;
	std::string textureName;
    
    Color ambientColor;
    Color diffuseColor;
    Color specularColor;
    Color emissionColor;
    
    float shininess;
    
    Color color;
    
public:
    
    Material(void);
    ~Material(void);

	void setAmbient(float, float, float);
	void setDiffuse(float, float, float);
	void setSpecular(float, float, float);
	void setEmission(float, float, float);

	void setTextureName(std::string);
	std::string getTextureName();
	void loadTexture();
	void setMaterialName(std::string);
	std::string getMaterialName();

	virtual void apply(void);

};

#endif
