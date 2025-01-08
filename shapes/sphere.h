#pragma once 

#include "core/hittable.h"
#include "core/ray.h"
#include "core/interval.h"


class Sphere : public HitTable
{
public:
    // Stationary Sphere 
    Sphere(const Point3 &static_center, F32 radius, std::shared_ptr<Material> mat);
    // Moving Sphere
    Sphere(const Point3 &center1, const Point3 &center2, F32 radius, std::shared_ptr<Material> mat);

    bool Hit(const Ray& r, Interval t, HitRecord& rec) const override;
    AABB BBox() const override { return bbox; }

private:
    static void GetSphereUV(const Point3 &p, F32 &u, F32 &v);

    Ray center;
    F32 radius;
    std::shared_ptr<Material> mat;
    AABB bbox;
};
