#pragma once

#include "texture.h"

namespace raytracing
{
class CheckerTexture : public Texture
{
public:
    CheckerTexture(Texture* t0, Texture* t1);
    virtual Vector3 GetColor(double u, double v, const Vector3& p) const;

private:
    Texture* odd_;
    Texture* even_;
};
}  // namespace raytracing
