#ifndef CONENODE_H
#define CONENODE_H

#include "Geode.h"

class ConeNode : public Geode
{
private:
	float base;
	float height;

public:
	ConeNode(float, float);
	virtual void render(void);
	virtual BoundBox update(void);
};

#endif