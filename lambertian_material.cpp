#include "lambertian_material.h"
#include "ortho_normal_basis.h"

namespace raytracing
{
LambertianMaterial::LambertianMaterial(Texture* albedo)
	: albedo_(albedo)
{
}

bool LambertianMaterial::Scatter(const Ray3& ray, const HitRecord& hit_record, ScatterRecord& scatter_record)
{
    scatter_record.is_specular = false;
    scatter_record.attenuation = albedo_->GetColor(hit_record.u, hit_record.v, hit_record.point);
    //pdf_.SetW(hit_record.normal);
    scatter_record.pdf = CosinePdf(hit_record.normal);
    return true;
}

double LambertianMaterial::ScatterPdf(const Ray3& ray, const HitRecord& hit_record, const Ray3& scattered) const
{
    double cosine = hit_record.normal.Dot(scattered.direction.UnitVector());
    return cosine < 0.0 ? 0.0 : cosine / M_PI;
}
}  // namespace raytracing
