#pragma once

#include "material.h"
#include "pdf.h"
#include "texture.h"

namespace raytracing
{
class LambertianMaterial : public Material
{
public:
	LambertianMaterial(Texture* albedo);
	bool Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered_ray) const override;

private:
	Texture* albedo_;
	CosinePdf pdf_;
};
}  // namespace raytracing
