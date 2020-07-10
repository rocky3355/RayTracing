#pragma once

#include "material.h"

namespace raytracing
{
class MetalMaterial : public Material
{
public:
	MetalMaterial(const Vector3& albedo, double fuzz);
	virtual bool Scatter(const Ray3& ray, const HitRecord& hit_record, ScatterRecord& scatter_record) const;

private:
	Vector3 albedo_;
	double fuzz_;
};
}  // namespace raytracing
