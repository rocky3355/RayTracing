#include "diffuse_light.h"

namespace raytracing
{
DiffuseLight::DiffuseLight(Texture* emit)
	: emit_(emit)
{
}

bool DiffuseLight::Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered_ray) const
{
	return false;
}

Vector3 DiffuseLight::Emit(const HitRecord& hit_record) const
{
    if (hit_record.front_face)
    {
        return emit_->GetColor(hit_record.u, hit_record.v, hit_record.point);
    }
    else
    {
        return VECTOR3_ZERO;
    }
}
}  // namespace raytracing
