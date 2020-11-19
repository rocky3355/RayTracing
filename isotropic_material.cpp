#include "isotropic_material.h"

namespace raytracing
{
IsotropicMaterial::IsotropicMaterial(Texture* albedo)
	: albedo_(albedo)
{
}

bool IsotropicMaterial::Scatter(const Ray3& ray, const HitRecord& hit_record, ScatterRecord& scatter_record) const
{
	scatter_record.is_specular = true;
	scatter_record.pdf = nullptr;
	scatter_record.specular_ray = Ray3(hit_record.point, Vector3::GetRandomInUnitSphere(), ray.time);
	scatter_record.attenuation = albedo_->GetColor(hit_record.u, hit_record.v, hit_record.point);
	return true;
}
}  // namespace raytracing
