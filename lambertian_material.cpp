#include "lambertian_material.h"

namespace raytracing
{
LambertianMaterial:: LambertianMaterial(std::shared_ptr<Texture> albedo)
	: albedo_(albedo)
{
}

bool LambertianMaterial::Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered) const
{
    Vector3 scatter_direction = hit_record.normal + Vector3::GetRandomUnitVector();
    scattered = Ray3(hit_record.point, scatter_direction, ray.time);
    attenuation = albedo_->GetColor(hit_record.u, hit_record.v, hit_record.point);
    return true;
}
}  // namespace raytracing
