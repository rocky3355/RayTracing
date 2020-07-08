#include "solid_color.h"

namespace raytracing
{
SolidColor::SolidColor()
{
}

SolidColor::SolidColor(Vector3 color)
    : color_(color)
{
}

SolidColor::SolidColor(double red, double green, double blue)
    : SolidColor(Vector3(red, green, blue))
{
}

Vector3 SolidColor::GetColor(double u, double v, const Vector3& p) const
{
    return color_;
}
}  // namespace raytracing
