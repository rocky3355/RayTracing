#pragma once

#include "material.h"

namespace raytracing
{
class DielectricMaterial : public Material
{
public:
	DielectricMaterial(double refraction_index);
	bool Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered_ray) const override;

private:
	double Schlick(double cosine, double refraction_idx) const;
	double refraction_index_;
};
}  // namespace raytracing
