#pragma once

#include "material.h"
#include "texture.h"

namespace raytracing
{
class DiffuseLight : public Material
{
public:
    DiffuseLight(std::shared_ptr<Texture> emit);
    virtual bool Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered) const;
    virtual Vector3 Emit(double u, double v, const Vector3& p) const;

private:
    std::shared_ptr<Texture> emit_;
};
}  // namespace raytracing
