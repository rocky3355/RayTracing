#pragma once

#include "material.h"
#include "texture.h"

namespace raytracing
{
class DiffuseLight : public Material
{
public:
    DiffuseLight(Texture* emit);
    bool Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered_ray) const override;
    Vector3 Emit(const HitRecord& hit_record) const override;

private:
    Texture* emit_;
};
}  // namespace raytracing
