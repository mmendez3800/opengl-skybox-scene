#include "ViewFrustum.h"
#include <iostream>

void ViewFrustum::update(float nearDist, float farDist, float ratio, float fov, Camera& c)
{
	float Hnear = 2 * tan(fov / 2) * nearDist;
	float Wnear = Hnear * ratio;

	float Hfar = 2 * tan(fov / 2) * farDist;
	float Wfar = Hfar * ratio;

	// getting more 'direction' instead of 'directed at'
	Vector3 d = (c.d - c.e).normalize();

	Vector3 fc = c.e + d * farDist;

	Vector3 right = d.cross(c.up).normalize();
	Vector3 up = right.cross(d).normalize();
	Vector3 ftl = fc + up * (Hfar / 2) - right * (Wfar / 2);
	Vector3 ftr = fc + up * (Hfar / 2) + right * (Wfar / 2);
	Vector3 fbl = fc - up * (Hfar / 2) - right * (Wfar / 2);
	Vector3 fbr = fc - up * (Hfar / 2) + right * (Wfar / 2);

	Vector3 nc = c.e + d * nearDist;

	Vector3 ntl = nc + up * (Hnear / 2) - right * (Wnear / 2);
	Vector3 ntr = nc + up * (Hnear / 2) + right * (Wnear / 2);
	Vector3 nbl = nc - up * (Hnear / 2) - right * (Wnear / 2);
	Vector3 nbr = nc - up * (Hnear / 2) + right * (Wnear / 2);

	planeNormals[0] = d.normalize() * -1;
	planeNormals[1] = d.normalize();
	planeNormals[2] = (nbl - fbl).cross(ftl - fbl).normalize();
	planeNormals[3] = (fbr - nbr).cross(ntr - nbr).normalize();
	planeNormals[4] = (ftl - ftr).cross(ntr - ftr).normalize();
	planeNormals[5] = (nbl - nbr).cross(fbr - nbr).normalize();

	planePoints[0] = nc.dot(planeNormals[0]);
	planePoints[1] = fc.dot(planeNormals[1]);
	planePoints[2] = fbl.dot(planeNormals[2]);
	planePoints[3] = nbr.dot(planeNormals[3]);
	planePoints[4] = ftr.dot(planeNormals[4]);
	planePoints[5] = nbr.dot(planeNormals[5]);
}

bool ViewFrustum::insideOrIntersect(Vector3& p, float& radius)
{
	Vector3* normal = &planeNormals[0];
	float* point = &planePoints[0];
	for (int i = 0; i < 6; ++i, ++normal, ++point)
	{
		float dist = normal->dot(p) - *point;
		if (dist > radius)
			return false;
	}
	return true;
}

bool FakeFrustum::insideOrIntersect(Vector3&, float&)
{
	return true;
}
