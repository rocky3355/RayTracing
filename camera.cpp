#include "camera.h"

namespace raytracing
{
Camera::Camera()
{
    auto aspect_ratio = 16.0 / 9.0;
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    origin_ = VECTOR3_ZERO;
    horizontal_ = Vector3(viewport_width, 0.0, 0.0);
    vertical_ = Vector3(0.0, viewport_height, 0.0);
    lower_left_corner_ = origin_ - horizontal_ / 2.0 - vertical_ / 2.0 - Vector3(0.0, 0.0, focal_length);
}

Ray3 Camera::GetRay(double u, double v) const
{
    return Ray3(origin_, lower_left_corner_ + u * horizontal_ + v * vertical_ - origin_);
}
}  // namespace raytracing
