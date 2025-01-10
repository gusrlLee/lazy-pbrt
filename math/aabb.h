#pragma once
#include "core/interval.h"
#include "core/ray.h"
// #include "math/vec3.h"
// #include "core/hittable.h"

class AABB
{
public:
    Interval x, y, z;

    // Construction
    AABB();
    AABB(const Interval &x, const Interval &y, const Interval &z);
    AABB(const Point3 &a, const Point3 &b);
    AABB(const AABB &box0, const AABB &box1);

    const Interval &AxisInterval(I32 n) const
    {
        if (n == 1)
            return y;
        if (n == 2)
            return z;
        return x;
    }

    int LongestAxis() const
    {
        // Returns the index of the longest axis of the bounding box.
        if (x.Size() > y.Size())
            return x.Size() > z.Size() ? 0 : 2;
        else
            return y.Size() > z.Size() ? 1 : 2;
    }

    bool Hit(const Ray &r, Interval t) const;

    static const AABB empty, universe;


private:
    void PadToMinimums()
    {
        F32 delta = 0.0001;
        if (x.Size() < delta) x = x.Expand(delta);
        if (y.Size() < delta) y = y.Expand(delta);
        if (z.Size() < delta) z = z.Expand(delta);
    }
};


AABB operator+(const AABB& bbox, const Vec3 &offset);
AABB operator+(const Vec3 &offset, const AABB &bbox);