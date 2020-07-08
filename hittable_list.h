#pragma once

#include <memory>
#include <vector>
#include "hittable.h"

namespace raytracing
{
class HittableList : public Hittable
{
public:
	HittableList();
	HittableList(std::shared_ptr<Hittable> object);
	void Clear();
	void Add(std::shared_ptr<Hittable> object);
	virtual bool Hit(const Ray3& r, double t_min, double t_max, HitRecord& hit_record) const;
    virtual bool CreateBoundingBox(double t_end, AABB& bounding_box) const;

	std::vector<std::shared_ptr<Hittable>> objects;
};
}  // namespace raytracing
