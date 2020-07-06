#include "dielectric_material.h"

namespace raytracing
{
DielectricMaterial::DielectricMaterial(double refraction_index)
    : refraction_index_(refraction_index)
{
}

bool DielectricMaterial::Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered) const
{
    attenuation = Vector3(1.0, 1.0, 1.0);
    double etai_over_etat;
    if (hit_record.front_face) {
        etai_over_etat = 1.0 / refraction_index_;
    }
    else {
        etai_over_etat = refraction_index_;
    }

    Vector3 unit_direction = ray.direction.UnitVector();
    double cos_theta = std::fmin(-unit_direction.Dot(hit_record.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
    if (etai_over_etat * sin_theta > 1.0) {
        Vector3 reflected = Vector3::Reflect(unit_direction, hit_record.normal);
        scattered = Ray3(hit_record.point, reflected);
        return true;
    }

    double reflect_probability = Schlick(cos_theta, etai_over_etat);
    if (GetRandomDouble() < reflect_probability)
    {
        Vector3 reflected = Vector3::Reflect(unit_direction, hit_record.normal);
        scattered = Ray3(hit_record.point, reflected);
        return true;
    }

    Vector3 refracted = Vector3::Refract(unit_direction, hit_record.normal, etai_over_etat);
    scattered = Ray3(hit_record.point, refracted);
    return true;
}

double DielectricMaterial::Schlick(double cosine, double refraction_idx) const
{
    double r0 = (1.0 - refraction_idx) / (1.0 + refraction_idx);
    double r0_squared = r0 * r0;
    return r0_squared + (1.0 - r0_squared) * std::pow((1.0 - cosine), 5.0);
}
}  // namespace raytracing
