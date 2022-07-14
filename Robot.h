#ifndef ROBOT_H
#define ROBOT_H

#include "Group.h"
#include "MatrixTransform.h"
#include "Geode.h"

class Robot : public Geode
{
private:
	Group * rootNode;
	MatrixTransform * leftArmRotate;
	MatrixTransform * rightArmRotate;
	MatrixTransform * leftLegRotate;
	MatrixTransform * rightLegRotate;
	float leg_height;
	float arm_height;
	float rotate_max;
	float rotate_current;
	float rotate_amount;

public:
	Robot();
	~Robot(void);
	virtual BoundBox update(void);
	void updateMovement(void);
	virtual void drawBoundingSphere(Matrix4&);
	virtual void render(void);
};

#endif // ROBOT_H
