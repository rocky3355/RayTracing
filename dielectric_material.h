#pragma once

#include "material.h"

namespace raytracing
{
class DielectricMaterial : public Material
{
public:
	DielectricMaterial(double refraction_index);
	virtual bool Scatter(const Ray3& ray, const HitRecord& hit_record, ScatterRecord& scatter_record);

private:
	double Schlick(double cosine, double refraction_idx) const;
	double refraction_index_;
};
}  // namespace raytracing
