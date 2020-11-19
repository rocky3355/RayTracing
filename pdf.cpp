#include "pdf.h"

namespace raytracing
{

// ##########################################

CosinePdf::CosinePdf()
{
}

CosinePdf::CosinePdf(const Vector3& w)
{
    uvw_.BuildFromW(w);
}

double CosinePdf::Value(const Vector3& direction) const
{
    double cosine = direction.UnitVector().Dot(uvw_.w());
    return (cosine <= 0) ? 0 : cosine / M_PI;
}

Vector3 CosinePdf::Generate() const
{
    return uvw_.Local(Vector3::GetRandomCosineDirection());
}

// ##########################################

HittablePdf::HittablePdf(Hittable* object, const Vector3& origin)
    : object_(object), origin_(origin)
{
}

double HittablePdf::Value(const Vector3& direction) const
{
    return object_->PdfValue(origin_, direction);
}

Vector3 HittablePdf::Generate() const
{
    return object_->GetRandom(origin_);
}

// ##########################################

// TODO: These can be static methods
double MixturePdf::Value(const Pdf& p0, const Pdf& p1, const Vector3& direction) const
{
    return 0.5 * p0.Value(direction) + 0.5 * p1.Value(direction);
}

Vector3 MixturePdf::Generate(const Pdf& p0, const Pdf& p1) const
{
    if (GetRandomDouble() < 0.5)
    {
        return p0.Generate();
    }
    else
    {
        return p1.Generate();
    }
}
}  // namespace raytracing
