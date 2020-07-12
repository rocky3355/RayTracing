#pragma once

#include "hittable.h"

namespace raytracing
{
class Translation : public Hittable
{
public:
    Translation(std::shared_ptr<Hittable> object, const Vector3& translation);
    virtual bool Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const;
    virtual bool CreateBoundingBox(double t_end, AABB& bounding_box) const;

private:
    std::shared_ptr<Hittable> object_;
    Vector3 translation_;
};
}  // namespace raytracing
