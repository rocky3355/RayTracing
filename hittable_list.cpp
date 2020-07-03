#include "hittable_list.h"

namespace raytracing
{
HittableList::HittableList() { }

HittableList::HittableList(std::shared_ptr<Hittable> object)
{
	Add(object);
}

void HittableList::Clear()
{
	objects.clear();
}

void HittableList::Add(std::shared_ptr<Hittable> object)
{
	objects.push_back(object);
}

bool HittableList::Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    for (const std::shared_ptr<Hittable>& object : objects)
    {
        if (object->Hit(ray, t_min, closest_so_far, temp_rec))
        {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            hit_record = temp_rec;
        }
    }

    return hit_anything;
}
} // namespace raytracing
