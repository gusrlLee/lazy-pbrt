#pragma once
#include "types.h"
#include "vec3.h"
#include "ray.h"
#include "interval.h"

#include "hit_record.h"
#include "primitive.h"
#include "material.h"

class Sphere : public Primitive
{
public:
    // Stationary Sphere 
    Sphere(const Point3 &static_center, f64 radius, std::shared_ptr<Material> mat) : center(static_center, Vec3(0, 0, 0)), radius(std::fmax(0, radius)), mat(mat) {}
    // Moving Sphere
    Sphere(const Point3 &center1, const Point3 &center2, f64 radius, std::shared_ptr<Material> mat) : center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(mat) {}

    bool hit(const Ray& r, Interval r_t, hit_record& rec) const override;

private:
    Ray center;
    f64 radius;
    std::shared_ptr<Material> mat;
};
