#pragma once

#include "hittable.h"
#include "texture.h"

namespace raytracing
{
class ConstantMedium : public Hittable
{
public:
    ConstantMedium(std::shared_ptr<Hittable> boundary, double density, std::shared_ptr<Texture> albedo);
    virtual bool Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const;
    virtual bool CreateBoundingBox(double t_end, AABB& bounding_box) const;

private:
    std::shared_ptr<Hittable> boundary_;
    std::shared_ptr<Material> phase_function_;
    double neg_inv_density_;
};
}  // namespace raytracing
