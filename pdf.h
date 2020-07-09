#pragma once

#include "ortho_normal_basis.h"

namespace raytracing
{
class Pdf
{
public:
    virtual ~Pdf();
    virtual double Value(const Vector3& direction) const = 0;
    virtual Vector3 Generate() const = 0;
};

class CosinePdf : public Pdf
{
public:
    CosinePdf(const Vector3& w);
    virtual double Value(const Vector3& direction) const;
    virtual Vector3 Generate() const;

private:
    OrthoNormalBasis uvw_;
};
}  // namespace raytracing
