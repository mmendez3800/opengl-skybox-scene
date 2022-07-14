#ifndef SPHERENODE_H
#define SPHERENODE_H

#include "Geode.h"
#include "Material.h"

class SphereNode : public Geode
{
private:
	float radius;
	Material material;

public:
	SphereNode(float);
	virtual void render(void);
	virtual BoundBox update(void);
};

#endif // SPHERENODE_H

