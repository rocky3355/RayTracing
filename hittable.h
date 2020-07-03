#pragma once

#include "ray3.h"

namespace raytracing
{
struct HitRecord
{
	Vector3 point;
	Vector3 normal;
	double t;
	bool front_face;

	inline void SetFaceNormal(const Ray3& ray, const Vector3& outward_normal)
	{
		front_face = ray.direction.Dot(outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class Hittable
{
public:
	virtual bool Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const = 0;
};
}  // namespace raytracing