#ifndef UPDATEDATA_H
#define UPDATEDATA_H

#include <iostream>
#include "Vector3.h"

#define MASK_NONE 0
#define MASK_TRANSLATE 0x1
#define MASK_ORBIT 0x2
#define MASK_RESET 0x4
#define MASK_SCALE 0x8

class UpdateData
{
    
public:
    
    double dt;
    //Place any objects here that you want to pass to an update() call
	Vector3 translate;
	unsigned int mask_command;
	float orbit_angle;
	float scale;

    
    UpdateData(void);
    virtual ~UpdateData(void);
    
};

#endif
