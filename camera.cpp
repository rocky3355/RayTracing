#include "camera.h"

namespace raytracing
{
Camera::Camera(Vector3 origin, Vector3 look_at, Vector3 up, double vertical_fov, double aspect_ratio, double aperture, double focus_distance, double time_start, double time_end)
    : origin_(origin), time_start_(time_start), time_end_(time_end)
{
    double theta = DegreesToRadians(vertical_fov);
    double h = std::tan(theta / 2);
    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;

    Vector3 w = (origin_ - look_at).UnitVector();
    Vector3 u = up.Cross(w).UnitVector();
    Vector3 v = w.Cross(u);

    horizontal_ = focus_distance * viewport_width * u;
    vertical_ = focus_distance * viewport_height * v;
    lower_left_corner_ = origin_ - horizontal_ / 2 - vertical_ / 2 - focus_distance * w;

    lens_radius_ = aperture / 2.0;
}

Ray3 Camera::GetRay(double u, double v) const
{
    Vector3 rd = lens_radius_ * Vector3::GetRandomInUnitDisk();
    Vector3 offset = u_ * rd.x() + v_ * rd.y();

    return Ray3(
        origin_ + offset,
        lower_left_corner_ + u * horizontal_ + v * vertical_ - origin_ - offset,
        GetRandomDouble(time_start_, time_end_)
    );
}
}  // namespace raytracing
