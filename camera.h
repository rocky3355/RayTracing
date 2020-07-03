#pragma once

#include "ray3.h"

namespace raytracing
{
class Camera
{
public:
    Camera();
    Ray3 GetRay(double u, double v) const;

private:
    Vector3 origin_;
    Vector3 lower_left_corner_;
    Vector3 horizontal_;
    Vector3 vertical_;
};
}  // namespace raytracing
