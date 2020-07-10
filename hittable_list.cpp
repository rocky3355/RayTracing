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

bool HittableList::CreateBoundingBox(double t_end, AABB& bounding_box) const
{
    if (objects.empty())
    {
        return false;
    }

    AABB temp_box;
    bool is_first_box = true;

    for (const std::shared_ptr<Hittable>& object : objects)
    {
        if (!object->CreateBoundingBox(t_end, temp_box))
        {
            return false;
        }
        bounding_box = is_first_box ? temp_box : AABB::GetSurroundingBox(bounding_box, temp_box);
        is_first_box = false;
    }

    return true;
}

double HittableList::PdfValue(const Vector3& origin, const Vector3& v) const
{
    double sum = 0.0;
    double weight = 1.0 / objects.size();
    for (const auto& object : objects)
    {
        sum += weight * object->PdfValue(origin, v);
    }
    return sum;
}

Vector3 HittableList::GetRandom(const Vector3& origin) const
{
    if (objects.size() == 0)
    {
        return Hittable::GetRandom(origin);
    }
    return objects[GetRandomInt(objects.size() - 1)]->GetRandom(origin);
}
} // namespace raytracing
