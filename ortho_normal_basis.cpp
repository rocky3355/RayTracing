#include "ortho_normal_basis.h"

namespace raytracing
{
OrthoNormalBasis::OrthoNormalBasis()
{
}

Vector3 OrthoNormalBasis::u() const
{
	return axis_[0];
}

Vector3 OrthoNormalBasis::v() const
{
	return axis_[1];
}

Vector3 OrthoNormalBasis::w() const
{
	return axis_[2];
}

Vector3 OrthoNormalBasis::Local(double a, double b, double c) const
{
    return a * u() + b * v() + c * w();
}

Vector3 OrthoNormalBasis::Local(const Vector3& a) const {
    return a.x() * u() + a.y() * v() + a.z() * w();
}

void OrthoNormalBasis::BuildFromW(const Vector3& n) {
    axis_[2] = n.UnitVector();
    Vector3 a = (std::fabs(w().x()) > 0.9) ? Vector3(0, 1, 0) : Vector3(1, 0, 0);
    axis_[1] = w().Cross(a).UnitVector();
    axis_[0] = w().Cross(v());
}
}  // namespace raytracing
