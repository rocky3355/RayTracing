#pragma once

#include "hittable.h"

namespace raytracing
{
class GlobalLight : public Hittable
{
public:
	GlobalLight(Vector3 color);
	virtual bool Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const;
	virtual bool CreateBoundingBox(double t_end, AABB& bounding_box) const;
	virtual double PdfValue(const Vector3& origin, const Vector3& v) const;

private:
	std::shared_ptr<Material> material_;
};
}  // namespace raytracing
