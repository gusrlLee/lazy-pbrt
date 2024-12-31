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
    Sphere(const Point3 &center, f64 radius, std::shared_ptr<Material> mat) : center(center), radius(std::fmax(0, radius)), mat(mat) {}
    bool hit(const Ray& r, Interval r_t, hit_record& rec) const override;

private:
    Point3 center;
    f64 radius;
    std::shared_ptr<Material> mat;
};
