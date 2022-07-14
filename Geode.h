#ifndef GEODE_H
#define GEODE_H

#include "Node.h"

class Geode : public Node
{
public:
	virtual void draw(Matrix4 &, ViewFrustum&);
	virtual void render(void) = 0;
	virtual BoundBox update(void) = 0;
	virtual void clearChildren(bool);
};

#endif GEODE_H