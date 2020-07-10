#include "hittable_pdf.h"

namespace raytracing
{
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
}  // namespace raytracing
