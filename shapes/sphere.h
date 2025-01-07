#pragma once 

#include "core/hitable.h"
#include "core/ray.h"
#include "core/interval.h"


class Sphere : public HitTable
{
public:
    // Stationary Sphere 
    Sphere(const Point3 &static_center, F32 radius, std::shared_ptr<Material> mat) : center(static_center, Vec3(0, 0, 0)), radius(std::fmax(0, radius)), mat(mat) {}
    // Moving Sphere
    Sphere(const Point3 &center1, const Point3 &center2, F32 radius, std::shared_ptr<Material> mat) : center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(mat) {}

    bool Hit(const Ray& r, Interval t, HitRecord& rec) const override;

private:
    Ray center;
    F32 radius;
    std::shared_ptr<Material> mat;
};
