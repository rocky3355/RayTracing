#pragma once

#include "hittable.h"
#include "ortho_normal_basis.h"

namespace raytracing
{
class Pdf
{
public:
    virtual double Value(const Vector3& direction) const = 0;
    virtual Vector3 Generate() const = 0;
};

class CosinePdf : public Pdf
{
public:
    CosinePdf();
    CosinePdf(const Vector3& w);
    double Value(const Vector3& direction) const override;
    Vector3 Generate() const override;

private:
    OrthoNormalBasis uvw_;
};

class HittablePdf : public Pdf
{
public:
    HittablePdf(Hittable* object, const Vector3& origin);
    double Value(const Vector3& direction) const override;
    Vector3 Generate() const override;

private:
    Vector3 origin_;
    Hittable* object_;
};

class MixturePdf
{
public:
    double Value(const Pdf& p0, const Pdf& p1, const Vector3& direction) const;
    Vector3 Generate(const Pdf& p0, const Pdf& p1) const;
};

}  // namespace raytracing
