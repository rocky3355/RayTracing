#pragma once

#include "ray3.h"

namespace raytracing
{
class AABB
{
public:
    AABB();
    AABB(const Vector3& min, const Vector3& max);
    bool Hit(const Ray3& ray, double t_min, double t_max) const;
    static AABB GetSurroundingBox(AABB box0, AABB box1)
    {
        Vector3 small(std::fmin(box0.min.x(), box1.min.x()),
            std::fmin(box0.min.y(), box1.min.y()),
            std::fmin(box0.min.z(), box1.min.z()));

        Vector3 big(std::fmax(box0.max.x(), box1.max.x()),
            std::fmax(box0.max.y(), box1.max.y()),
            std::fmax(box0.max.z(), box1.max.z()));

        return AABB(small, big);
    }

    Vector3 min;
    Vector3 max;
};
}  // namespace raytracing
