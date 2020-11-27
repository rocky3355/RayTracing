#include "lambertian_material.h"
#include "ortho_normal_basis.h"

namespace raytracing
{
LambertianMaterial::LambertianMaterial(Texture* albedo)
	: albedo_(albedo)
{
}

bool LambertianMaterial::Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered_ray) const
{
    Vector3 scatter_direction = hit_record.normal + Vector3::GetRandom();

    // Catch degenerate scatter direction
    if (scatter_direction.IsAlmostZero())
    {
        scatter_direction = hit_record.normal;
    }

    scattered_ray = Ray3(hit_record.point, scatter_direction, ray.time);
    attenuation = albedo_->GetColor(hit_record.u, hit_record.v, hit_record.point);
    return true;
}
}  // namespace raytracing
