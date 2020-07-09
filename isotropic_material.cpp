#include "isotropic_material.h"

namespace raytracing
{
IsotropicMaterial::IsotropicMaterial(std::shared_ptr<Texture> albedo)
	: albedo_(albedo)
{
}

bool IsotropicMaterial::Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered) const
{
	scattered = Ray3(hit_record.point, Vector3::GetRandomInUnitSphere(), ray.time);
	attenuation = albedo_->GetColor(hit_record.u, hit_record.v, hit_record.point);
	return true;
}
}  // namespace raytracing
