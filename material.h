#pragma once

#include "hittable.h"
#include "pdf.h"

namespace raytracing
{
struct ScatterRecord
{
    Ray3 specular_ray;
    bool is_specular;
    Vector3 attenuation;
    std::shared_ptr<Pdf> pdf;
};

class Material
{
public:
    virtual Vector3 Emit(const Ray3& ray, const HitRecord& hit_record) const
    {
        return VECTOR3_ZERO;
    }
    
    virtual bool Scatter(const Ray3& ray, const HitRecord& hit_record, ScatterRecord& scatter_record) const
    {
        return false;
    }
    
    virtual double ScatterPdf(const Ray3& ray, const HitRecord& hit_record, const Ray3& scattered) const
    {
        return 0.0;
    }
};
}
