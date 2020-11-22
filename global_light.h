#pragma once

#include "material.h"
#include "texture.h"

namespace raytracing
{
class GlobalLight : public Material
{
public:
    GlobalLight(const Vector3& color);
    virtual bool Scatter(const Ray3& ray, const HitRecord& hit_record, Vector3& attenuation, Ray3& scattered);
    virtual Vector3 Emit(const Ray3& ray, const HitRecord& hit_record) const;

private:
    Vector3 color_;
};
}  // namespace raytracing
