#include "global_light.h"

namespace raytracing
{
GlobalLight::GlobalLight(const Vector3& color)
    : color_(color)
{
}

bool GlobalLight::Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered)
{
    return false;
}

Vector3 GlobalLight::Emit(const Ray3& ray, const HitRecord& hit_record) const
{
    return color_;
}
}  // namespace raytracing
