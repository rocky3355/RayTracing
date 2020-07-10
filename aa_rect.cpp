#include "aa_rect.h"

namespace raytracing
{
XYRect::XYRect(double x0, double x1, double y0, double y1, double k, std::shared_ptr<Material> material)
	: x0_(x0), x1_(x1), y0_(y0), y1_(y1), k_(k), material_(material)
{
}

bool XYRect::Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const
{
    double t = (k_ - ray.origin.z()) / ray.direction.z();
    if (t < t_min || t > t_max)
    {
        return false;
    }

    double x = ray.origin.x() + t * ray.direction.x();
    double y = ray.origin.y() + t * ray.direction.y();
    if (x < x0_ || x > x1_ || y < y0_ || y > y1_)
    {
        return false;
    }

    hit_record.u = (x - x0_) / (x1_ - x0_);
    hit_record.v = (y - y0_) / (y1_ - y0_);
    hit_record.t = t;

    Vector3 outward_normal = Vector3(0.0, 0.0, 1.0);
    hit_record.SetFaceNormal(ray, outward_normal);
    hit_record.material = material_;
    hit_record.point = ray.At(t);
    return true;
}

bool XYRect::CreateBoundingBox(double t_end, AABB& bounding_box) const
{
	bounding_box = AABB(Vector3(x0_, y0_, k_ - 0.0001), Vector3(x1_, y1_, k_ + 0.0001));
	return true;
}

XZRect::XZRect(double x0, double x1, double z0, double z1, double k, std::shared_ptr<Material> material)
    : x0_(x0), x1_(x1), z0_(z0), z1_(z1), k_(k), material_(material)
{
}

bool XZRect::Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const
{
    double t = (k_ - ray.origin.y()) / ray.direction.y();
    if (t < t_min || t > t_max)
    {
        return false;
    }

    double x = ray.origin.x() + t * ray.direction.x();
    double z = ray.origin.z() + t * ray.direction.z();
    if (x < x0_ || x > x1_ || z < z0_ || z > z1_)
    {
        return false;
    }

    hit_record.u = (x - x0_) / (x1_ - x0_);
    hit_record.v = (z - z0_) / (z1_ - z0_);
    hit_record.t = t;

    Vector3 outward_normal = Vector3(0, 1, 0);
    hit_record.SetFaceNormal(ray, outward_normal);
    hit_record.material = material_;
    hit_record.point = ray.At(t);
    return true;
}

bool XZRect::CreateBoundingBox(double t_end, AABB& bounding_box) const
{
    bounding_box = AABB(Vector3(x0_, k_ - 0.0001, z0_), Vector3(x1_, k_ + 0.0001, z1_));
    return true;
}

double XZRect::PdfValue(const Vector3& origin, const Vector3& v) const
{
    HitRecord hit_record;
    if (!this->Hit(Ray3(origin, v), 0.001, Infinity, hit_record))
    {
        return 0.0;
    }

    double area = (x1_ - x0_) * (z1_ - z0_);
    double distance_squared = hit_record.t * hit_record.t * v.GetLengthSquared();
    double cosine = std::fabs(v.Dot(hit_record.normal) / v.GetLength());

    return distance_squared / (cosine * area);
}

Vector3 XZRect::GetRandom(const Vector3& origin) const
{
    Vector3 random_point = Vector3(GetRandomDouble(x0_, x1_), k_, GetRandomDouble(z0_, z1_));
    return random_point - origin;
}

YZRect::YZRect(double y0, double y1, double z0, double z1, double k, std::shared_ptr<Material> material)
    : y0_(y0), y1_(y1), z0_(z0), z1_(z1), k_(k), material_(material)
{
}

bool YZRect::Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const
{
    double t = (k_ - ray.origin.x()) / ray.direction.x();
    if (t < t_min || t > t_max)
    {
        return false;
    }

    double y = ray.origin.y() + t * ray.direction.y();
    double z = ray.origin.z() + t * ray.direction.z();
    if (y < y0_ || y > y1_ || z < z0_ || z > z1_)
    {
        return false;
    }

    hit_record.u = (y - y0_) / (y1_ - y0_);
    hit_record.v = (z - z0_) / (z1_ - z0_);
    hit_record.t = t;

    Vector3 outward_normal = Vector3(1, 0, 0);
    hit_record.SetFaceNormal(ray, outward_normal);
    hit_record.material = material_;
    hit_record.point = ray.At(t);
    return true;
}

bool YZRect::CreateBoundingBox(double t_end, AABB& bounding_box) const
{
    bounding_box = AABB(Vector3(k_ - 0.0001, y0_, z0_), Vector3(k_ + 0.0001, y1_, z1_));
    return true;
}
}  // namespace raytracing
