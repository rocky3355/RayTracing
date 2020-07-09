#include "pdf.h"

namespace raytracing
{
Pdf::~Pdf()
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
}  // namespace raytracing
