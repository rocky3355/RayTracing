#pragma once

#include "material.h"
#include "texture.h"

namespace raytracing
{
class DiffuseLight : public Material
{
public:
    DiffuseLight(Texture* emit);
    virtual bool Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered);
    virtual Vector3 Emit(const Ray3& ray, const HitRecord& hit_record) const;

private:
    Texture* emit_;
};
}  // namespace raytracing
