#include "Group.h"

Group::~Group(void)
{
	std::vector<Node*>::iterator itr = children.begin();
	while (itr != children.end())
	{
		delete *itr++;
	}
}


void Group::addChild(Node * child)
{
	children.push_back(child);
}

void Group::clearChildren(bool del)
{
	if (del)
	{
		for (std::vector<Node*>::iterator itr = children.begin(); itr != children.end(); ++itr)
		{
			(*itr)->clearChildren(del);
			delete *itr;
		}
	}
	children.clear();
}

void Group::removeChild(Node * child, bool del)
{

	for (std::vector<Node*>::iterator itr = children.begin(); itr != children.end(); ++itr)
	{
		if (*itr == child)
		{
			if (del)
			{
				(*itr)->clearChildren(del);
				delete *itr;
			}
			children.erase(itr);
			return;
		}
	}
}

void Group::draw(Matrix4 & C, ViewFrustum& frust)
{
	for (std::vector<Node*>::iterator itr = children.begin(); itr != children.end(); ++itr)
	{
		(*itr)->checkDraw(C, frust);
	}
}

BoundBox Group::update(void)
{
	BoundBox box;
	for (std::vector<Node*>::iterator itr = children.begin(); itr != children.end(); ++itr)
	{
		box.growToFit((*itr)->update());
	}
	box.standardize();
	box.calculateSphere(bound_point, bound_radius);
	return box;
}

void Group::drawBoundingSphere(Matrix4 & C)
{
	for (std::vector<Node*>::iterator itr = children.begin(); itr != children.end(); ++itr)
	{
		(*itr)->drawBoundingSphere(C);
	}
}

void Group::drawChildrenSpheres(Matrix4& C)
{
	for (std::vector<Node*>::iterator itr = children.begin(); itr != children.end(); ++itr)
	{
		(*itr)->forceDrawBoundSphere(C);
	}
}
