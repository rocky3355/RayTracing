#pragma once

#include "texture.h"

namespace raytracing
{
class CheckerTexture : public Texture
{
public:
    CheckerTexture();
    CheckerTexture(std::shared_ptr<Texture> t0, std::shared_ptr<Texture> t1);
    virtual Vector3 GetColor(double u, double v, const Vector3& p) const;

private:
    std::shared_ptr<Texture> odd_;
    std::shared_ptr<Texture> even_;
};
}  // namespace raytracing
