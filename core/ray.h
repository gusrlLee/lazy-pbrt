#pragma once 

#include "core/types.h"
#include "math/vec3.h"

class Ray
{
public:
    Ray() {}

    Ray(const Point3 &origin, const Vec3 &direction, F32 time) : o(origin), d(direction), tm(time) {}
    Ray(const Point3 &origin, const Vec3 &direction) : Ray(origin, direction, 0) {}

    const Point3 &Orig() const { return o; }
    const Vec3 &Dir() const { return d; }
    F32 Time() const { return tm; }

    Point3 At(F32 t) const
    {
        return o + t * d;
    }

private:
    Point3 o;
    Vec3 d;
    F32 tm;
};
