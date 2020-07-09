#pragma once
#include "material.h"
#include "texture.h"

namespace raytracing
{
class IsotropicMaterial : public Material
{
public:
    IsotropicMaterial(std::shared_ptr<Texture> albedo);
    virtual bool Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered) const;

private:
    std::shared_ptr<Texture> albedo_;
};
}  // namespace raytracing
