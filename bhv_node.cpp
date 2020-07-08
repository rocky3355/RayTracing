#include "bhv_node.h"
#include <algorithm>
#include <functional>

namespace raytracing
{
BhvNode::BhvNode()
{
}

BhvNode::BhvNode(HittableList& list, double time_end)
    : BhvNode(list.objects, 0, list.objects.size(), time_end)
{
}

BhvNode::BhvNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, double time_end)
{
    int axis = GetRandomInt(2);
    auto comparator = (axis == 0) ? &BhvNode::BoxXCompare : (axis == 1) ? &BhvNode::BoxYCompare : &BhvNode::BoxZCompare;

    size_t object_span = end - start;

    if (object_span == 1)
    {
        left_ = right_ = objects[start];
    }
    else if (object_span == 2)
    {
        if (comparator(objects[start], objects[start + 1]))
        {
            left_ = objects[start];
            right_ = objects[start + 1];
        }
        else
        {
            left_ = objects[start + 1];
            right_ = objects[start];
        }
    }
    else {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);
        auto mid = start + object_span / 2;
        left_ = std::make_shared<BhvNode>(objects, start, mid, time_end);
        right_ = std::make_shared<BhvNode>(objects, mid, end, time_end);
    }

    AABB box_left, box_right;

    if (!left_->CreateBoundingBox(time_end, box_left) || !right_->CreateBoundingBox(time_end, box_right))
    {
        std::cerr << "No bounding box in bvh_node constructor.\n";
    }

    bounding_box_ = AABB::GetSurroundingBox(box_left, box_right);
}

bool BhvNode::Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const
{
    if (!bounding_box_.Hit(ray, t_min, t_max))
    {
        return false;
    }

    bool hit_left = left_->Hit(ray, t_min, t_max, hit_record);
    bool hit_right = right_->Hit(ray, t_min, hit_left ? hit_record.t : t_max, hit_record);

    return hit_left || hit_right;
}

bool BhvNode::CreateBoundingBox(double t_end, AABB& output_box) const
{
    output_box = bounding_box_;
    return true;
}
}
