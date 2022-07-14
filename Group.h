#ifndef GROUP_H
#define GROUP_H

#include "Node.h"
#include <vector>

class Group : public Node
{
protected:
	std::vector<Node*> children;

public:
	~Group(void);
	void addChild(Node*);
	void removeChild(Node*, bool);
	void clearChildren(bool);
	virtual BoundBox update(void);
	virtual void draw(Matrix4&, ViewFrustum&);
	virtual void drawBoundingSphere(Matrix4&);
	void drawChildrenSpheres(Matrix4&);
};

#endif // GROUP_H
