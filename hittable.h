#pragma once

#include <memory>
#include "aabb.h"

namespace raytracing
{

class Material;

struct HitRecord
{
	Vector3 point;
	Vector3 normal;
	std::shared_ptr<Material> material;
	double t;
	double u;
	double v;
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
    virtual bool CreateBoundingBox(double t_end, AABB& bounding_box) const = 0;
};
}  // namespace raytracing
