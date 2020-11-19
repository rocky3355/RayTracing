#include "translation.h"

namespace raytracing
{
Translation::Translation(Hittable* object, const Vector3& translation)
    : object_(object), translation_(translation)
{

}

bool Translation::Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const
{
    Ray3 moved_ray(ray.origin - translation_, ray.direction, ray.time);
    if (!object_->Hit(moved_ray, t_min, t_max, hit_record))
    {
        return false;
    }

    hit_record.point += translation_;
    hit_record.SetFaceNormal(moved_ray, hit_record.normal);

    return true;
}

bool Translation::CreateBoundingBox(double t_end, AABB& bounding_box) const
{
    if (!object_->CreateBoundingBox(t_end, bounding_box))
    {
        return false;
    }

    bounding_box = AABB(bounding_box.min + translation_, bounding_box.max + translation_);
    return true;
}
}  // namespace raytracing
