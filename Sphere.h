#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include "Drawable.h"

class Sphere : public Drawable
{
    
public:
    
    double radius;
    int slices, stacks;
    
    Sphere(double, int, int);
    
    virtual void draw(DrawData&);
    virtual void update(UpdateData&);
	virtual float getScaleFactor(void);
    
};

#endif
