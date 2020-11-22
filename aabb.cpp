#include "aabb.h"

namespace raytracing
{
AABB::AABB()
{
}

AABB::AABB(const Vector3& min, const Vector3& max)
    : min(min), max(max)
{
}

bool AABB::Hit(const Ray3& ray, double t_min, double t_max) const
{
    for (int i = 0; i < 3; ++i)
    {
        double invD = 1.0f / ray.direction[i];
        double t0 = (min[i] - ray.origin[i]) * invD;
        double t1 = (max[i] - ray.origin[i]) * invD;

        if (invD < 0.0f)
        {
            std::swap(t0, t1);
        }

        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;

        if (t_max <= t_min)
        {
            return false;
        }
    }

    return true;
}
}  // namespace raytracing
