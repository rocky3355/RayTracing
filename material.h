#pragma once

#include "hittable.h"

namespace raytracing
{
class Material
{
public:
    virtual Vector3 Emit(double u, double v, const Vector3& p) const
    {
        return VECTOR3_ZERO;
    }
    virtual bool Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered) const = 0;
};
}
