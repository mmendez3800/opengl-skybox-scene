#ifndef VeCTOR2_H
#define VeCTOR2_H

#include <string>

class Vector2
{

protected:

	float m[2];

public:

	Vector2();
	Vector2(float, float);
	virtual ~Vector2(void);

	float* ptr();

	void set(float, float);
	void set(int index, float value);
	float& operator [] (int);

	void print(std::string);

};

#endif