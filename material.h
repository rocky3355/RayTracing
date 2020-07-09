#pragma once

#include "hittable.h"

namespace raytracing
{
class Material
{
public:
    virtual Vector3 Emit(const Ray3& ray, const HitRecord& hit_record) const
    {
        return VECTOR3_ZERO;
    }
    
    virtual bool Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered, double& pdf) const
    {
        return false;
    }
    
    virtual double ScatterPdf(const Ray3& ray, const HitRecord& hit_record, Ray3& scattered) const
    {
        return 0.0;
    }
};
}
