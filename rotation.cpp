#include "rotation.h"
#include <limits>

namespace raytracing
{
RotationY::RotationY(Hittable* object, double angle)
    : object_(object)
{
    double radians = DegreesToRadians(angle);
    sin_theta_ = std::sin(radians);
    cos_theta_ = std::cos(radians);
    // TODO: 1.0 as t_end hardcoded?
    has_bounding_box_ = object_->CreateBoundingBox(1.0, bounding_box_);

    Vector3 min(Infinity, Infinity, Infinity);
    Vector3 max(-Infinity, -Infinity, -Infinity);

    for (int i = 0; i < 2; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            for (int k = 0; k < 2; ++k)
            {
                double x = i * bounding_box_.max.x() + (1 - i) * bounding_box_.min.x();
                double y = j * bounding_box_.max.y() + (1 - j) * bounding_box_.min.y();
                double z = k * bounding_box_.max.z() + (1 - k) * bounding_box_.min.z();

                double newx = cos_theta_ * x + sin_theta_ * z;
                double newz = -sin_theta_ * x + cos_theta_ * z;

                Vector3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++)
                {
                    min[c] = std::fmin(min[c], tester[c]);
                    max[c] = std::fmax(max[c], tester[c]);
                }
            }
        }
    }

    bounding_box_ = AABB(min, max);
}

bool RotationY::Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const
{
    Vector3 origin = ray.origin;
    Vector3 direction = ray.direction;

    origin[0] = cos_theta_ * ray.origin[0] - sin_theta_ * ray.origin[2];
    origin[2] = sin_theta_ * ray.origin[0] + cos_theta_ * ray.origin[2];

    direction[0] = cos_theta_ * ray.direction[0] - sin_theta_ * ray.direction[2];
    direction[2] = sin_theta_ * ray.direction[0] + cos_theta_ * ray.direction[2];

    Ray3 rotated_ray(origin, direction, ray.time);

    if (!object_->Hit(rotated_ray, t_min, t_max, hit_record))
    {
        return false;
    }

    Vector3 p = hit_record.point;
    Vector3 normal = hit_record.normal;

    p[0] = cos_theta_ * hit_record.point[0] + sin_theta_ * hit_record.point[2];
    p[2] = -sin_theta_ * hit_record.point[0] + cos_theta_ * hit_record.point[2];

    normal[0] = cos_theta_ * hit_record.normal[0] + sin_theta_ * hit_record.normal[2];
    normal[2] = -sin_theta_ * hit_record.normal[0] + cos_theta_ * hit_record.normal[2];

    hit_record.point = p;
    hit_record.SetFaceNormal(rotated_ray, normal);

    return true;
}

bool RotationY::CreateBoundingBox(double t_end, AABB& bounding_box) const
{
    bounding_box = bounding_box_;
    return has_bounding_box_;
}
}  // namespace raytracing
