#pragma once 

#include "core/hittable.h"
#include "core/ray.h"
#include "core/interval.h"

#include "core/onb.h"
#include "core/random.h"

class Sphere : public HitTable
{
public:
    // Stationary Sphere 
    Sphere(const Point3 &static_center, F32 radius, std::shared_ptr<Material> mat);
    // Moving Sphere
    Sphere(const Point3 &center1, const Point3 &center2, F32 radius, std::shared_ptr<Material> mat);

    bool Hit(const Ray& r, Interval t, HitRecord& rec) const override;
    AABB BBox() const override { return bbox; }

    F32 PdfValue(const Point3 &origin, const Vec3 &dir) const override;
    Vec3 Random(const Point3 &origin) const override 
    {
        Vec3 direction = center.At(0) - origin;
        auto distSq = direction.LengthSqaured();
        OrthonormalBasis uvw(direction);
        return uvw.Transform(Random::ToSphere(radius, distSq));
    }


private:
    static void GetSphereUV(const Point3 &p, F32 &u, F32 &v);

    Ray center;
    F32 radius;
    std::shared_ptr<Material> mat;
    AABB bbox;
};
