#include "constant_medium.h"
#include "isotropic_material.h"

namespace raytracing
{
ConstantMedium::ConstantMedium(std::shared_ptr<Hittable> boundary, double density, std::shared_ptr<Texture> albedo)
	: boundary_(boundary), neg_inv_density_(-1.0 / density)
{
	phase_function_ = std::make_shared<IsotropicMaterial>(albedo);
}

bool ConstantMedium::Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const
{
    // Print occasional samples when debugging. To enable, set enableDebug true.
    //const bool enableDebug = false;
    //const bool debugging = enableDebug && random_double() < 0.00001;

    HitRecord rec1, rec2;

    if (!boundary_->Hit(ray, -Infinity, Infinity, rec1))
    {
        return false;
    }

    if (!boundary_->Hit(ray, rec1.t + 0.0001, Infinity, rec2))
    {
        return false;
    }

    //if (debugging) std::cerr << "\nt0=" << rec1.t << ", t1=" << rec2.t << '\n';

    if (rec1.t < t_min)
    {
        rec1.t = t_min;
    }
    if (rec2.t > t_max)
    {
        rec2.t = t_max;
    }

    if (rec1.t >= rec2.t)
    {
        return false;
    }

    if (rec1.t < 0.0)
    {
        rec1.t = 0.0;
    }

    const double ray_length = ray.direction.GetLength();
    const double distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const double hit_distance = neg_inv_density_ * std::log(GetRandomDouble());

    if (hit_distance > distance_inside_boundary)
    {
        return false;
    }

    hit_record.t = rec1.t + hit_distance / ray_length;
    hit_record.point = ray.At(hit_record.t);
    hit_record.normal = Vector3(1.0, 0.0, 0.0);  // arbitrary
    hit_record.front_face = true;                // also arbitrary
    hit_record.material = phase_function_;

    return true;
}

bool ConstantMedium::CreateBoundingBox(double t_end, AABB& bounding_box) const
{
	return boundary_->CreateBoundingBox(t_end, bounding_box);
}
}  // namespace raytracing
