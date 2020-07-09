#include "lambertian_material.h"
#include "ortho_normal_basis.h"

namespace raytracing
{
LambertianMaterial::LambertianMaterial(std::shared_ptr<Texture> albedo)
	: albedo_(albedo)
{
}

bool LambertianMaterial::Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered, double& pdf) const
{
    OrthoNormalBasis uvw;
    uvw.BuildFromW(hit_record.normal);
    Vector3 direction = uvw.Local(Vector3::GetRandomCosineDirection());
    scattered = Ray3(hit_record.point, direction.UnitVector(), ray.time);
    attenuation = albedo_->GetColor(hit_record.u, hit_record.v, hit_record.point);
    pdf = uvw.w().Dot(scattered.direction) / M_PI;
    return true;
}

double LambertianMaterial::ScatterPdf(const Ray3& ray, const HitRecord& hit_record, Ray3& scattered) const
{
    double cosine = hit_record.normal.Dot(scattered.direction.UnitVector());
    return cosine < 0.0 ? 0.0 : cosine / M_PI;
}
}  // namespace raytracing
