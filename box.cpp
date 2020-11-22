#include "box.h"
#include "aa_rect.h"
#include "flip_face.h"

namespace raytracing
{
Box::Box(const Vector3& p0, const Vector3& p1, Material* material)
{
    box_min_ = p0;
    box_max_ = p1;

    sides_.Add(new XYRect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), material));
    sides_.Add(new FlipFace(new XYRect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), material)));

    sides_.Add(new XZRect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), material));
    sides_.Add(new FlipFace(new XZRect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), material)));

    sides_.Add(new YZRect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), material));
    sides_.Add(new FlipFace(new YZRect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), material)));
}

bool Box::Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const
{
    return sides_.Hit(ray, t_min, t_max, hit_record);
}

bool Box::CreateBoundingBox(double t_end, AABB& bounding_box) const
{
    bounding_box = AABB(box_min_, box_max_);
    return true;
}
}  // namespace raytracing
