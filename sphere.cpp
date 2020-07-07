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
            return true;
        }
    }

    return false;
}
}  // namespace raytracing
