#include "diffuse_light.h"

namespace raytracing
{
DiffuseLight::DiffuseLight(std::shared_ptr<Texture> emit)
	: emit_(emit)
{
}

bool DiffuseLight::Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered) const
{
	return false;
}

Vector3 DiffuseLight::Emit(double u, double v, const Vector3& p) const
{
	return emit_->GetColor(u, v, p);
}
}  // namespace raytracing
