#pragma once

#include "hittable_list.h"

namespace raytracing
{
class Box : public Hittable
{
public:
    Box(const Vector3& p0, const Vector3& p1, Material* material);
    virtual bool Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const;
    virtual bool CreateBoundingBox(double t_end, AABB& bounding_box) const;

private:
    Vector3 box_min_;
    Vector3 box_max_;
    HittableList sides_;
};
}  // namespace raytracing
