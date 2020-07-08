#include "noise_texture.h"

namespace raytracing
{
NoiseTexture::NoiseTexture(double scale)
    : scale_(scale)
{
}

Vector3 NoiseTexture::GetColor(double u, double v, const Vector3& p) const
{
    return VECTOR3_UNIT_XYZ * 0.5 * (1.0 + std::sin(scale_ * p.z() + 10.0 * perlin_.Turbulence(p)));
}
}  // namespace raytracing
