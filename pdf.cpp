#include "pdf.h"

namespace raytracing
{
Pdf::~Pdf()
{
    // TODO?
}

// ##########################################

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

HittablePdf::HittablePdf(std::shared_ptr<Hittable> object, const Vector3& origin)
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

MixturePdf::MixturePdf(std::shared_ptr<Pdf> p0, std::shared_ptr<Pdf> p1)
{
    p_[0] = p0;
    p_[1] = p1;
}

double MixturePdf::Value(const Vector3& direction) const
{
    return 0.5 * p_[0]->Value(direction) + 0.5 * p_[1]->Value(direction);
}

Vector3 MixturePdf::Generate() const
{
    if (GetRandomDouble() < 0.5)
    {
        return p_[0]->Generate();
    }
    else
    {
        return p_[1]->Generate();
    }
}
}  // namespace raytracing
