#include "math/aabb.h"
#include "aabb.h"

const AABB AABB::empty    = AABB(Interval::empty,    Interval::empty,    Interval::empty);
const AABB AABB::universe = AABB(Interval::universe, Interval::universe, Interval::universe);

AABB::AABB() {} // The default AABB is empty

AABB::AABB(const Interval &x, const Interval &y, const Interval &z) : x(x), y(y), z(z) {}

AABB::AABB(const Point3 &a, const Point3 &b)
{
    x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
    y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
    z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
}

AABB::AABB(const AABB &box0, const AABB &box1)
{
    x = Interval(box0.x, box1.x);
    y = Interval(box0.y, box1.y);
    z = Interval(box0.z, box1.z);
}
bool AABB::Hit(const Ray &r, Interval t) const
{
    const Point3 &ro = r.Orig(); // ray origin
    const Vec3 &rd = r.Dir();    // ray direction

    for (I32 axis = 0; axis < 3; axis++)
    {
        const Interval &ax = AxisInterval(axis);
        const F32 adinv = 1.0 / rd[axis];

        auto t0 = (ax.min - ro[axis]) * adinv;
        auto t1 = (ax.max - ro[axis]) * adinv;

        if (t0 < t1)
        {
            if (t0 > t.min)
                t.min = t0;
            if (t1 < t.max)
                t.max = t1;
        }
        else
        {
            if (t1 > t.min)
                t.min = t1;
            if (t0 < t.max)
                t.max = t0;
        }

        if (t.max <= t.min)
            return false;
    }
    return true;
}
