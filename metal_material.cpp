#include "metal_material.h"

namespace raytracing
{
MetalMaterial::MetalMaterial(const Vector3& albedo, double fuzz)
    : albedo_(albedo), fuzz_(fuzz < 1.0 ? fuzz : 1.0)
{
}

bool MetalMaterial::Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered) const
{
    Vector3 reflected = Vector3::Reflect(ray.direction.UnitVector(), hit_record.normal);
    scattered = Ray3(hit_record.point, reflected + fuzz_ * Vector3::GetRandomInUnitSphere());
    attenuation = albedo_;
    return (scattered.direction.Dot(hit_record.normal) > 0);
}
}  // namespace raytracing
