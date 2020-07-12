#pragma once

#include "perlin.h"
#include "texture.h"

namespace raytracing
{
class NoiseTexture : public Texture
{
public:
    NoiseTexture(double scale = 1.0);
    virtual Vector3 GetColor(double u, double v, const Vector3& p) const;

private:
    double scale_;
    Perlin perlin_;
};
}  // namespace raytracing
