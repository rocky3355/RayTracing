#pragma once

#include "vector3.h"

namespace raytracing
{
class Texture
{
public:
	virtual Vector3 GetColor(double u, double v, const Vector3& p) const = 0;
};
}  // namespace raytracing
