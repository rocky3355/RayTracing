#pragma once

#include "vector3.h"

namespace raytracing
{
class Ray3
{
public:
	Ray3();
	Ray3(const Vector3& origin, const Vector3& direction, double time = 0.0);

	Vector3 At(double t) const;

	Vector3 origin;
	Vector3 direction;
	double time;
};
}  // namespace raytracing

//#endif  // RAY3_H
