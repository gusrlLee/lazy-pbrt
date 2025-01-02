#pragma once

#include "types.h"
#include "vec3.h"

class Ray
{
public:
    Ray() {}

    Ray(const Point3 &origin, const Vec3 &direction, f64 time) : orig(origin), dir(direction), tm(time) {}
    Ray(const Point3 &origin, const Vec3 &direction) : Ray(origin, direction, 0) {}

    const Point3 &origin() const { return orig; }
    const Vec3 &direction() const { return dir; }
    f64 time() const { return tm; }

    Point3 at(f64 t) const
    {
        return orig + t * dir;
    }

private:
    Point3 orig;
    Vec3 dir;
    f64 tm;
};
