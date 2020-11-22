#include "always_hit.h"

namespace raytracing
{
AlwaysHit::AlwaysHit(Material* material)
    : material_(material)
{
}

double AlwaysHit::PdfValue(const Vector3& origin, const Vector3& v) const
{
    HitRecord hit_record;
    hit_record.front_face = true;
    hit_record.material = material_;
    return 1.0;
}

Vector3 AlwaysHit::GetRandom(const Vector3& origin) const
{
    // TODO
    /*Vector3 random_point = Vector3(GetRandomDouble(x0_, x1_), k_, GetRandomDouble(z0_, z1_));
    return random_point - origin;*/
    return VECTOR3_UNIT_XYZ;
}

bool AlwaysHit::Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const
{
    /*hit_record.front_face = true;
    hit_record.material = material_;*/
    return true;
}

bool AlwaysHit::CreateBoundingBox(double t_end, AABB& bounding_box) const
{
    //bounding_box.SetIsAlwaysHit(true);
    return true;
}
}  // namespace raytracing
