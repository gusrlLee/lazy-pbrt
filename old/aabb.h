#pragma once 
#include "types.h"
#include "interval.h"
#include "vec3.h"
#include "ray.h"

class AABB 
{
public:
    Interval x, y, z;

    // Constructor 
    AABB() {}
    AABB(const Interval &x, const Interval &y, const Interval &z) :
        x(x), y(y), z(z) {}

    AABB(const Point3 &a, const Point3 &b);

    const Interval& axis_interval(i32 n) const 
    {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    bool hit(const Ray &r, Interval ray_t) const;

};
