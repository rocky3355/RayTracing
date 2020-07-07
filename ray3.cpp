#include "ray3.h"

namespace raytracing
{
Ray3::Ray3() {}

Ray3::Ray3(const Vector3& origin, const Vector3& direction, double time)
	: origin(origin), direction(direction), time(time)
{
}

Vector3 Ray3::At(double t) const
{
	return Vector3(origin + t * direction);
}
}  // namespace raytracing
