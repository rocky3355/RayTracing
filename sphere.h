#pragma once

#include "hittable.h"

namespace raytracing
{
class Sphere : public Hittable
{
public:
	Sphere();
	Sphere(Vector3 center, double radius);
	virtual bool Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const;

	Vector3 center;
	double radius;
};
}  // namespace raytracing
