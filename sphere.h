#pragma once

#include "hittable.h"

namespace raytracing
{
class Sphere : public Hittable
{
public:
	Sphere();
	Sphere(Vector3 center, double radius, std::shared_ptr<Material> material, const Vector3& velocity = VECTOR3_ZERO);
	virtual bool Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const;
	std::shared_ptr<Material> material;

private:
	Vector3 origin_;
	double radius_;
	Vector3 velocity_;
};
}  // namespace raytracing
