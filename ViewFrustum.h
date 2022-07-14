#ifndef VIEWFRUSTUM_H
#define VIEWFRUSTUM_H

#include "Vector3.h"
#include "Camera.h"

class ViewFrustum
{
private:
	Vector3 planeNormals[6];
	float planePoints[6];

public:
	void update(float nearDist, float farDist, float ratio, float fov, Camera& c);
	virtual bool insideOrIntersect(Vector3& p, float& radius);
};

class FakeFrustum : public ViewFrustum
{
	virtual bool insideOrIntersect(Vector3&, float&);
};

#endif // VIEWFRUSTUM_H
