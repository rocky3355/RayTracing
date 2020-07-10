#pragma once

#include "material.h"
#include "texture.h"

namespace raytracing
{
class LambertianMaterial : public Material
{
public:
	LambertianMaterial(std::shared_ptr<Texture> albedo);
	virtual bool Scatter(const Ray3& ray, const HitRecord& hit_record, ScatterRecord& scatter_record) const;
	virtual double ScatterPdf(const Ray3& ray, const HitRecord& hit_record, const Ray3& scattered) const;

private:
	std::shared_ptr<Texture> albedo_;
};
}  // namespace raytracing
