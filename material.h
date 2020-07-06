#pragma once

#include "hittable.h"

namespace raytracing
{
class Material
{
public:
    virtual bool Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered) const = 0;
};
}
