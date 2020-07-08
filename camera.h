#pragma once

#include "ray3.h"

namespace raytracing
{
class Camera
{
public:
    Camera(Vector3 origin, Vector3 look_at, Vector3 up, double vertical_fov, double aspect_ratio, double aperture, double focus_distance, double time_end);
    Ray3 GetRay(double u, double v) const;

private:
    Vector3 origin_;
    Vector3 lower_left_corner_;
    Vector3 horizontal_;
    Vector3 vertical_;
    Vector3 u_;
    Vector3 v_;
    double lens_radius_;
    double time_end_;
};
}  // namespace raytracing
