#pragma once

#include "hittable.h"
#include "pdf.h"

namespace raytracing
{
class Material
{
public:
    virtual Vector3 Emit(const HitRecord& hit_record) const
    {
        return VECTOR3_ZERO;
    }
    
    virtual bool Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered_ray) const
    {
        return false;
    }
};
}
