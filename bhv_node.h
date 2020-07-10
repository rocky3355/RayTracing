#pragma once

#include "hittable_list.h"

namespace raytracing
{
class BhvNode : public Hittable
{
public:
    BhvNode();
    BhvNode(HittableList& list, double time_end);
    BhvNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, double time_end);
    virtual bool Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const;
    virtual bool CreateBoundingBox(double t_end, AABB& output_box) const;
    //virtual double PdfValue(const Vector3& origin, const Vector3& v) const;
    //virtual Vector3 GetRandom(const Vector3& origin) const;

private:
    static bool BoxCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b, int axis)
    {
        AABB box_a;
        AABB box_b;

        if (!a->CreateBoundingBox(0.0, box_a) || !b->CreateBoundingBox(0.0, box_b))
        {
            std::cerr << "No bounding box in bvh_node constructor.\n";
        }
        return box_a.min.data[axis] < box_b.min.data[axis];
    }

    static bool BoxXCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
    {
        return BoxCompare(a, b, 0);
    }

    static bool BoxYCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
    {
        return BoxCompare(a, b, 1);
    }

    static bool BoxZCompare(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
    {
        return BoxCompare(a, b, 2);
    }

    std::shared_ptr<Hittable> left_;
    std::shared_ptr<Hittable> right_;
    AABB bounding_box_;
};
}  // namespace raytracing
