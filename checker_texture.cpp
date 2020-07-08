#include "checker_texture.h"

namespace raytracing
{
CheckerTexture::CheckerTexture()
{
}

CheckerTexture::CheckerTexture(std::shared_ptr<Texture> tex0, std::shared_ptr<Texture> tex1)
    : even_(tex0), odd_(tex1)
{
}

Vector3 CheckerTexture::GetColor(double u, double v, const Vector3& p) const
{
    double sines = std::sin(10 * p.x()) * std::sin(10 * p.y()) * std::sin(10 * p.z());
    if (sines < 0)
    {
        return odd_->GetColor(u, v, p);
    }
    return even_->GetColor(u, v, p);
}
}  // namespace raytracing
