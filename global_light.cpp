#include "global_light.h"
#include "lambertian_material.h"
#include "solid_color.h"

namespace raytracing
{
GlobalLight::GlobalLight(Vector3 color)
{
	material_ = std::make_shared<LambertianMaterial>(std::make_shared<SolidColor>(color));
}

bool GlobalLight::Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const
{
	hit_record.front_face = true;
	hit_record.material = material_;
	hit_record.normal = -ray.direction.UnitVector(); // TODO: Unit vector required?
	//hit_record.point = ; // TODO
	return true;
}

bool GlobalLight::CreateBoundingBox(double t_end, AABB& bounding_box) const
{
	return false;
}

double GlobalLight::PdfValue(const Vector3& origin, const Vector3& v) const
{
	return 1.0;
}
}  // namespace raytracing
