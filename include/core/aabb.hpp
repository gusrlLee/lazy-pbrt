#ifndef GI_CORE_AABB_HPP
#define GI_CORE_AABB_HPP

#include "core/geometry.hpp"

namespace gi
{
    class AABB
    {
    public:
        Interval x, y, z;

        AABB() {} // The default AABB is empty, since intervals are empty by default.

        AABB(const Interval &x, const Interval &y, const Interval &z)
            : x(x), y(y), z(z)
        {
            PadToMinimums();
        }

        AABB(const Point3 &a, const Point3 &b)
        {
            // Treat the two points a and b as extrema for the bounding box, so we don't require a
            // particular minimum/maximum coordinate order.

            x = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
            y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
            z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);
            PadToMinimums();
        }

        AABB(const AABB &box0, const AABB &box1)
        {
            x = Interval(box0.x, box1.x);
            y = Interval(box0.y, box1.y);
            z = Interval(box0.z, box1.z);
        }

        const Interval &AxisInterval(int n) const
        {
            if (n == 1)
                return y;
            if (n == 2)
                return z;
            return x;
        }

        Bool Hit(const Ray &r, Interval t) const
        {
            const Point3 &rOrig = r.Orig();
            const Vec3 &rDir = r.Dir();

            for (int axis = 0; axis < 3; axis++)
            {
                const Interval &ax = AxisInterval(axis);
                const double adinv = 1.0 / rDir[axis];

                auto t0 = (ax.min - rOrig[axis]) * adinv;
                auto t1 = (ax.max - rOrig[axis]) * adinv;

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

        int LongestAxis() const
        {
            // Returns the index of the longest axis of the bounding box.

            if (x.Size() > y.Size())
                return x.Size() > z.Size() ? 0 : 2;
            else
                return y.Size() > z.Size() ? 1 : 2;
        }

        static const AABB empty, universe;

    private:
        void PadToMinimums()
        {
            // Adjust the AABB so that no side is narrower than some delta, padding if necessary.

            Float delta = 0.0001;
            if (x.Size() < delta)
                x = x.Expand(delta);
            if (y.Size() < delta)
                y = y.Expand(delta);
            if (z.Size() < delta)
                z = z.Expand(delta);
        }
    };

    const AABB AABB::empty = AABB(Interval::empty, Interval::empty, Interval::empty);
    const AABB AABB::universe = AABB(Interval::universe, Interval::universe, Interval::universe);

    AABB operator+(const AABB &bbox, const Vec3 &offset)
    {
        return AABB(bbox.x + offset.x(), bbox.y + offset.y(), bbox.z + offset.z());
    }

    AABB operator+(const Vec3 &offset, const AABB &bbox)
    {
        return bbox + offset;
    }
}

#endif