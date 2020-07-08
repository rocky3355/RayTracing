#include "sphere.h"

namespace raytracing
{
Sphere::Sphere() { }

Sphere::Sphere(Vector3 origin, double radius, std::shared_ptr<Material> material, const Vector3& velocity)
	: origin_(origin), radius_(radius), material(material), velocity_(velocity)
{
}

bool Sphere::Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const
{
    Vector3 timed_origin = origin_ + velocity_ * ray.time;
	Vector3 oc = ray.origin - timed_origin;
	double a = ray.direction.GetLengthSquared();
	double half_b = oc.Dot(ray.direction);
	double c = oc.GetLengthSquared() - radius_ * radius_;
	double discriminant = half_b * half_b - a * c;

    if (discriminant > 0)
    {
        double root = std::sqrt(discriminant);
        double temp = (-half_b - root) / a;
        double temp2 = (-half_b + root) / a;
        if ((temp < t_max && temp > t_min) || (temp2 < t_max && temp > t_min))
        {
            hit_record.t = temp;
            hit_record.point = ray.At(hit_record.t);
            Vector3 outward_normal = (hit_record.point - timed_origin) / radius_;
            hit_record.SetFaceNormal(ray, outward_normal);
            hit_record.material = material;
            GetTextureCoordinates((hit_record.point - origin_) / radius_, hit_record.u, hit_record.v);
            return true;
        }
    }

    return false;
}

bool Sphere::CreateBoundingBox(double t_end, AABB& bounding_box) const
{
    Vector3 origin_end = origin_ + velocity_ * t_end;
    Vector3 radius_vec(radius_, radius_, radius_);

    AABB box0(origin_ - radius_vec, origin_ + radius_vec);
    AABB box1(origin_end - radius_vec, origin_end + radius_vec);

    bounding_box = AABB::GetSurroundingBox(box0, box1);
    return true;
}

void Sphere::GetTextureCoordinates(const Vector3& p, double& u, double& v) const
{
    double phi = std::atan2(p.z(), p.x());
    double theta = std::asin(p.y());
    u = 1 - (phi + M_PI) / (2 * M_PI);
    v = (theta + M_PI / 2) / M_PI;
}
}  // namespace raytracing
