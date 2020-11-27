#include "dielectric_material.h"

namespace raytracing
{
DielectricMaterial::DielectricMaterial(double refraction_index)
    : refraction_index_(refraction_index)
{
}

bool DielectricMaterial::Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered_ray) const
{
    attenuation = VECTOR3_UNIT_XYZ;
    double refraction_ratio = hit_record.front_face ? (1.0 / refraction_index_) : refraction_index_;

    Vector3 unit_direction = ray.direction.UnitVector();
    double cos_theta = std::fmin(-unit_direction.Dot(hit_record.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = refraction_ratio * sin_theta > 1.0;
    Vector3 direction;

    if (cannot_refract || Schlick(cos_theta, refraction_ratio) > GetRandomDouble())
        direction = Vector3::Reflect(unit_direction, hit_record.normal);
    else
        direction = Vector3::Refract(unit_direction, hit_record.normal, refraction_ratio);

    scattered_ray = Ray3(hit_record.point, direction, ray.time);
    return true;
}

double DielectricMaterial::Schlick(double cosine, double refraction_idx) const
{
    double r0 = (1.0 - refraction_idx) / (1.0 + refraction_idx);
    double r0_squared = r0 * r0;
    return r0_squared + (1.0 - r0_squared) * std::pow((1.0 - cosine), 5.0);
}
}  // namespace raytracing
