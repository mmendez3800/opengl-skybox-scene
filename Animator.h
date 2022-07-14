#ifndef ANIMATOR_H
#define ANIMATOR_H

#include "Matrix4.h"
#include "MatrixTransform.h"
#include <vector>
#include <iostream>

class Animation
{
protected:
	long start_time;
	long end_time;
	MatrixTransform * mod_transform;
	Matrix4 * modify;
	Matrix4 modification;

public:
	Animation(long, long, Matrix4, Matrix4*);
	Animation(long, long, Matrix4, MatrixTransform*);
	virtual void tick(void);
	bool viableTime(long);
};

class YRotateAnimation : public Animation
{
private:
	float perTickRotate;

public:
	YRotateAnimation(long, long, float, MatrixTransform*);
};

class TranslateAnimation : public Animation
{
public:
	TranslateAnimation(long, long, Vector3, MatrixTransform*);
};

class InitialState
{
private:
	Matrix4* matrix;
	MatrixTransform* mat_trans;
	Matrix4 state;

public:
	InitialState(Matrix4*, Matrix4);
	InitialState(MatrixTransform*, Matrix4);
	void enact(void);
};

class Animator
{
private:
	long current_time;
	bool started;
	std::vector<Animation> animation_list;
	std::vector<InitialState> initial_list;

public:
	void tick(void);
	void addAnimation(Animation);
	void addInitial(InitialState);
	void resetToInitial(void);
	void start(void);
};

#endif // ANIMATOR_H
