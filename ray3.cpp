#include "ray3.h"

namespace raytracing
{
Ray3::Ray3() {}

Ray3::Ray3(const Vector3& origin, const Vector3& direction)
	: origin(origin), direction(direction)
{
}

Vector3 Ray3::At(double t) const
{
	return Vector3(origin + t * direction);
}
}  // namespace raytracing
