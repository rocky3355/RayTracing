#pragma once

#include "vector3.h"

namespace raytracing
{
class OrthoNormalBasis
{
public:
    OrthoNormalBasis();
    Vector3 Local(double a, double b, double c) const;
    Vector3 Local(const Vector3& a) const;
    void BuildFromW(const Vector3& n);
    Vector3 u() const;
    Vector3 v() const;
    Vector3 w() const;
    inline Vector3 operator[](int i) const { return axis_[i]; }

private:
    Vector3 axis_[3];
};
}  // namespace raytracing
