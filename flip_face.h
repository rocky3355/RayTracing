#pragma once

#include "hittable.h"

namespace raytracing
{
class FlipFace : public Hittable
{
public:
    FlipFace(std::shared_ptr<Hittable> object);
    virtual bool Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const;
    virtual bool CreateBoundingBox(double t_end, AABB& bounding_box) const;

private:
    std::shared_ptr<Hittable> object_;
};
}  // namespace raytracing
