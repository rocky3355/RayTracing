#pragma once

#include "material.h"

namespace raytracing
{
class MetalMaterial : public Material
{
public:
	MetalMaterial(const Vector3& albedo, double fuzz);
	bool Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered_ray) const override;

private:
	Vector3 albedo_;
	double fuzz_;
};
}  // namespace raytracing
