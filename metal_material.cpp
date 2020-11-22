#include "metal_material.h"

namespace raytracing
{
MetalMaterial::MetalMaterial(const Vector3& albedo, double fuzz)
    : albedo_(albedo), fuzz_(fuzz < 1.0 ? fuzz : 1.0)
{
}

bool MetalMaterial::Scatter(const Ray3& ray, const HitRecord& hit_record, ScatterRecord& scatter_record)
{
    Vector3 reflected = Vector3::Reflect(ray.direction.UnitVector(), hit_record.normal);
    scatter_record.specular_ray = Ray3(hit_record.point, reflected + fuzz_ * Vector3::GetRandomInUnitSphere());
    scatter_record.attenuation = albedo_;
    scatter_record.is_specular = true;
    return true;
}
}  // namespace raytracing
