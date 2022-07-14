#ifndef CUBENODE_H
#define CUBENODE_H

#include "Geode.h"
#include "Material.h"

class CubeNode : public Geode
{
private:
	float length;
	Material material;

public:
	CubeNode(float);
	virtual void render(void);
	virtual BoundBox update(void);
};

#endif // CUBENODE_H
