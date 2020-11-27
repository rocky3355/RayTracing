#pragma once
#include "material.h"
#include "texture.h"

namespace raytracing
{
class IsotropicMaterial : public Material
{
public:
    IsotropicMaterial(Texture* albedo);
    bool Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered_ray) const override;

private:
    Texture* albedo_;
};
}  // namespace raytracing
