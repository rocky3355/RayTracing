#pragma once

#include "hittable.h"
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

class HittablePdf : public Pdf
{
public:
    HittablePdf(Hittable* object, const Vector3& origin);
    virtual double Value(const Vector3& direction) const;
    virtual Vector3 Generate() const;

private:
    Vector3 origin_;
    Hittable* object_;
};

class MixturePdf : public Pdf
{
public:
    MixturePdf(Pdf* p0, Pdf* p1);
    virtual double Value(const Vector3& direction) const;
    virtual Vector3 Generate() const;

private:
    Pdf* p_[2];
};

}  // namespace raytracing
