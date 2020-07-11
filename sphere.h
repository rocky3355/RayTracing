#pragma once
#include "hittable.h"

namespace raytracing
{
class Sphere : public Hittable
{
public:
	Sphere(Vector3 center, double radius, std::shared_ptr<Material> material, const Vector3& velocity = VECTOR3_ZERO);
	virtual bool Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const;
    virtual bool CreateBoundingBox(double t_end, AABB& bounding_box) const;
	virtual double PdfValue(const Vector3& origin, const Vector3& v) const;
	virtual Vector3 GetRandom(const Vector3& origin) const;

private:
	void GetTextureCoordinates(const Vector3& p, double& u, double& v) const;
	Vector3 origin_;
	double radius_;
	Vector3 velocity_;
	std::shared_ptr<Material> material_;
};
}  // namespace raytracing
