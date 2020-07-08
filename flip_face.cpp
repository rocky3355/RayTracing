#include "flip_face.h"

namespace raytracing
{
FlipFace::FlipFace(std::shared_ptr<Hittable> object)
	: object_(object)
{
}

bool FlipFace::Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const
{
	if (!object_->Hit(ray, t_min, t_max, hit_record))
	{
		return false;
	}
	hit_record.front_face = !hit_record.front_face;
	return true;
}

bool FlipFace::CreateBoundingBox(double t_end, AABB& bounding_box) const
{
	return object_->CreateBoundingBox(t_end, bounding_box);
}
}  // namespace raytracing
