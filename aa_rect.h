#pragma once

#include "hittable.h"
#include "material.h"

namespace raytracing
{
class XYRect : public Hittable
{
public:
    XYRect(double x0, double x1, double y0, double y1, double k, std::shared_ptr<Material> material);
    virtual bool Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const;
    virtual bool CreateBoundingBox(double t_end, AABB& output_box) const;

private:
    std::shared_ptr<Material> material_;
    double x0_;
    double x1_;
    double y0_;
    double y1_;
    double k_;
};

class XZRect : public Hittable
{
public:
    XZRect(double x0, double x1, double z0, double z1, double k, std::shared_ptr<Material> material);
    virtual bool Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const;
    virtual bool CreateBoundingBox(double t_end, AABB& bounding_box) const;
    virtual double PdfValue(const Vector3& origin, const Vector3& v) const;
    virtual Vector3 GetRandom(const Vector3& origin) const;

private:
    std::shared_ptr<Material> material_;
    double x0_;
    double x1_;
    double z0_;
    double z1_;
    double k_;
};

class YZRect : public Hittable
{
public:
    YZRect(double y0, double y1, double z0, double z1, double k, std::shared_ptr<Material> material);
    virtual bool Hit(const Ray3& ray, double t_min, double t_max, HitRecord& hit_record) const;
    virtual bool CreateBoundingBox(double t_end, AABB& bounding_box) const;

private:
    std::shared_ptr<Material> material_;
    double y0_;
    double y1_;
    double z0_;
    double z1_;
    double k_;
};
}  // namespace raytracing
