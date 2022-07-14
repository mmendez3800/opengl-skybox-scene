#include "Robot.h"

#define _USE_MATH_DEFINES

#include <math.h>
#include "SphereNode.h"
#include "CubeNode.h"
#include "ConeNode.h"
#include "Vector3.h"

Robot::Robot() : rootNode(new Group()), leftArmRotate(new MatrixTransform()), rightArmRotate(new MatrixTransform()),
leftLegRotate(new MatrixTransform()), rightLegRotate(new MatrixTransform()), rotate_max((float)(M_PI / 6)), rotate_current(0.0f), rotate_amount((float)(M_PI / 144)),
leg_height(2.0f), arm_height(1.5f)
{
	bound_point = Vector3(0.0f, -0.6f, 0.0f);
	bound_radius = 2.8f;

	Matrix4 scale;
	Matrix4 translate;
	Matrix4 rotate;
	MatrixTransform * scaleTransform;
	MatrixTransform * translateTransform;
	Geode * part;

	// Body
	scale.identity();	// not used here
	translate.identity();	// not used here
	part = new CubeNode(2.2f);
	rootNode->addChild(part);

	// Head
	scale.identity();
	translate.makeTranslate(Vector3(0.0f, 1.6f, 0.0f));
	translateTransform = new MatrixTransform(translate);
	part = new SphereNode(0.5f);
	translateTransform->addChild(part);
	rootNode->addChild(translateTransform);

	// Left leg
	scale.makeScale(1.0f, leg_height, 1.0f); // Only scale to appropriate height
	translate.makeTranslate(Vector3(-0.6f, -leg_height / 2.0f - 1.1f, 0.0f)); // Left by half width + 0.01, down by leg height, no z
	scaleTransform = new MatrixTransform(scale);
	translateTransform = new MatrixTransform(translate);
	part = new CubeNode(1.0f);
	scaleTransform->addChild(part);
	leftLegRotate->addChild(scaleTransform);
	translateTransform->addChild(leftLegRotate);
	rootNode->addChild(translateTransform);

	// Right leg
	scale.makeScale(1.0f, leg_height, 1.0f);
	translate.makeTranslate(Vector3(0.6f, -leg_height / 2.0f - 1.1f, 0.0f));
	translateTransform = new MatrixTransform(translate);
	scaleTransform = new MatrixTransform(scale);
	part = new CubeNode(1.0f);
	scaleTransform->addChild(part);
	rightLegRotate->addChild(scaleTransform);
	translateTransform->addChild(rightLegRotate);
	rootNode->addChild(translateTransform);

	// Left arm
	scale.makeScale(1.0f, arm_height / 2.0f, 1.0f);
	translate.makeTranslate(Vector3(-1.6f, 0.0f, 0.0f));
	rotate.makeRotateX((float)(-M_PI / 2));
	scaleTransform = new MatrixTransform(scale);
	translateTransform = new MatrixTransform(translate);
	leftArmRotate->setMatrix(rotate);
	part = new ConeNode(0.5f, 1.0f);
	scaleTransform->addChild(part);
	leftArmRotate->addChild(scaleTransform);
	translateTransform->addChild(leftArmRotate);
	rootNode->addChild(translateTransform);

	// Right arm
	scale.makeScale(1.0f, arm_height / 2.0f, 1.0f);
	translate.makeTranslate(Vector3(1.6f, 0.0f, 0.0f));
	rotate.makeRotateZ((float)(-M_PI / 2));
	scaleTransform = new MatrixTransform(scale);
	translateTransform = new MatrixTransform(translate);
	rightArmRotate->setMatrix(rotate);
	part = new ConeNode(0.5f, 1.0f);
	scaleTransform->addChild(part);
	rightArmRotate->addChild(scaleTransform);
	translateTransform->addChild(rightArmRotate);
	rootNode->addChild(translateTransform);
}

Robot::~Robot(void)
{
	rootNode->clearChildren(true);
	delete rootNode;
}

void Robot::updateMovement(void)
{
	Matrix4 translate_leg_down;
	Matrix4 translate_leg_up;

	translate_leg_down.makeTranslate(Vector3(0.0f, -leg_height / 2.0f, 0.0f));
	translate_leg_up.makeTranslate(Vector3(0.0f, leg_height / 2.0f, 0.0f));

	Matrix4 translate_arm_down;
	Matrix4 translate_arm_up;

	translate_arm_down.makeTranslate(Vector3(0.0f, -arm_height / 2.0f, 0.0f));
	translate_arm_up.makeTranslate(Vector3(0.0f, arm_height / 2.0f, 0.0f));

	Matrix4 rotate;
	rotate.makeRotateX(rotate_amount);

	rightLegRotate->preMultMatrix(translate_leg_up * rotate * translate_leg_down);
	leftArmRotate->preMultMatrix(translate_arm_up * rotate * translate_arm_down);

	rotate.makeRotateX(-rotate_amount);

	leftLegRotate->preMultMatrix(translate_leg_up * rotate * translate_leg_down);
	rightArmRotate->preMultMatrix(translate_arm_up * rotate * translate_arm_down);

	rotate_current += rotate_amount;

	if (rotate_current >= rotate_max || rotate_current <= -rotate_max)
	{
		rotate_amount = -rotate_amount;
	}
}

void Robot::render(void)
{
	Matrix4 I;
	FakeFrustum frust;
	I.identity();
	rootNode->draw(I, frust);
}

BoundBox Robot::update(void)
{
	return rootNode->update();
}

void Robot::drawBoundingSphere(Matrix4& C)
{
	rootNode->forceDrawBoundSphere(C);
	rootNode->drawChildrenSpheres(C);
}
