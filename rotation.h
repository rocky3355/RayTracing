#pragma once

#include "hittable.h"

namespace raytracing
{
class RotationY : public Hittable
{
public:
    RotationY(std::shared_ptr<Hittable> object, double angle);
    virtual bool Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const;
    virtual bool CreateBoundingBox(double t_end, AABB& bounding_box) const;

public:
    std::shared_ptr<Hittable> object_;
    double sin_theta_;
    double cos_theta_;
    bool has_bounding_box_;
    AABB bounding_box_;
};
}  // namespace raytracing
