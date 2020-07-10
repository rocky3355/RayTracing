#pragma once

#include "hittable.h"

namespace raytracing
{
class HittablePdf : public Hittable
{
public:
    HittablePdf(std::shared_ptr<Hittable> object, const Vector3& origin);
    virtual double Value(const Vector3& direction) const;
    virtual Vector3 Generate() const;

private:
    Vector3 origin_;
    std::shared_ptr<Hittable> object_;
};
}  // namespace raytracing
