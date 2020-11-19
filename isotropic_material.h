#pragma once
#include "material.h"
#include "texture.h"

namespace raytracing
{
class IsotropicMaterial : public Material
{
public:
    IsotropicMaterial(Texture* albedo);
    virtual bool Scatter(const Ray3& ray, const HitRecord& hit_record, ScatterRecord& scatter_record) const;

private:
    Texture* albedo_;
};
}  // namespace raytracing
