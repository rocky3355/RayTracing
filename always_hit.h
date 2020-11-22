#pragma once

#include "hittable.h"

namespace raytracing
{
class AlwaysHit : public Hittable
{
public:
    AlwaysHit(Material* material);
    virtual bool Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const;
    virtual bool CreateBoundingBox(double t_end, AABB& bounding_box) const;
    virtual double PdfValue(const Vector3& origin, const Vector3& v) const;
    virtual Vector3 GetRandom(const Vector3& origin) const;

private:
    Material* material_;
};
}  // namespace raytracing
