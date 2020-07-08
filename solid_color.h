#pragma once

#include "texture.h"

namespace raytracing
{
class SolidColor : public Texture
{
public:
    SolidColor();
    SolidColor(Vector3 color);
    SolidColor(double red, double green, double blue);
    virtual Vector3 GetColor(double u, double v, const Vector3& p) const;

private:
    Vector3 color_;
};
}  // namespace raytracing
