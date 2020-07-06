#pragma once

#include "material.h"

namespace raytracing
{
class LambertianMaterial : public Material
{
public:
	LambertianMaterial(const Vector3& albedo);
	virtual bool Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered) const;

private:
	Vector3 albedo_;
};
}  // namespace raytracing
