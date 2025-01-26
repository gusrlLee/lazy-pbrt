#ifndef GI_CORE_GEOMETRY_HPP
#define GI_CORE_GEOMETRY_HPP

#include "core/types.hpp"

namespace gi
{
    inline Float DegreeToRadians(Float degrees)
    {
        return degrees * PI / 180.0;
    }

    // Vector
    class Vec2
    {
    public:
        Float e[2];

        Vec2() : e{0, 0} {}
        Vec2(Float v) : e{v, v} {}
        Vec2(Float e0, Float e1) : e{e0, e1} {}

        Float x() const { return e[0]; }
        Float y() const { return e[1]; }

        Vec2 operator-() const { return Vec2(-e[0], -e[1]); }
        Float operator[](int i) const { return e[i]; }
        Float &operator[](int i) { return e[i]; }

        Vec2 &operator+=(const Vec2 &v)
        {
            e[0] += v.e[0];
            e[1] += v.e[1];
            return *this;
        }

        Vec2 &operator*=(Float t)
        {
            e[0] *= t;
            e[1] *= t;
            return *this;
        }

        Vec2 &operator/=(Float t)
        {
            return *this *= 1 / t;
        }

        Float Length() const
        {
            return std::sqrt(LengthSquared());
        }

        Float LengthSquared() const
        {
            return e[0] * e[0] + e[1] * e[1];
        }
    };

    class Vec3
    {
    public:
        Float e[3];

        Vec3() : e{0, 0, 0} {}
        Vec3(Float v) : e{v, v, v} {}
        Vec3(Float e0, Float e1, Float e2) : e{e0, e1, e2} {}

        Float x() const { return e[0]; }
        Float y() const { return e[1]; }
        Float z() const { return e[2]; }

        Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
        Float operator[](int i) const { return e[i]; }
        Float &operator[](int i) { return e[i]; }

        Vec3 &operator+=(const Vec3 &v)
        {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        Vec3 &operator*=(Float t)
        {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        Vec3 &operator/=(Float t)
        {
            return *this *= 1 / t;
        }

        Float Length() const
        {
            return std::sqrt(LengthSquared());
        }

        Float LengthSquared() const
        {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }

        Bool IsNearZero() const
        {
            // Return true if the vector is close to zero in all dimensions.
            auto s = 1e-8;
            return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
        }
    };

    // point3 is just an alias for Vec3, but useful for geometric clarity in the code.
    using Poitn2 = Vec2;
    using Point3 = Vec3;
    using Vertex = Vec3;

    using Normal = Vec3;
    using Color = Vec3;

    // Vector Utility Functions
    inline Vec3 operator+(const Vec3 &u, const Vec3 &v)
    {
        return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
    }

    inline Vec3 operator-(const Vec3 &u, const Vec3 &v)
    {
        return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
    }

    inline Vec3 operator*(const Vec3 &u, const Vec3 &v)
    {
        return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
    }

    inline Vec3 operator*(Float t, const Vec3 &v)
    {
        return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
    }

    inline Vec3 operator*(const Vec3 &v, Float t)
    {
        return t * v;
    }

    inline Vec3 operator/(const Vec3 &v, Float t)
    {
        return (1 / t) * v;
    }

    inline Float Dot(const Vec3 &u, const Vec3 &v)
    {
        return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
    }

    inline Vec3 Cross(const Vec3 &u, const Vec3 &v)
    {
        return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                    u.e[2] * v.e[0] - u.e[0] * v.e[2],
                    u.e[0] * v.e[1] - u.e[1] * v.e[0]);
    }

    inline Vec3 Normalize(const Vec3 &v)
    {
        return v / v.Length();
    }

    inline Vec3 Reflect(const Vec3 &v, const Vec3 &n)
    {
        return v - 2 * Dot(v, n) * n;
    }

    inline Vec3 Refract(const Vec3 &uv, const Vec3 &n, Float etaiOverEtat)
    {
        auto cosTheta = std::fmin(Dot(-uv, n), 1.0);
        Vec3 rOutPerp = etaiOverEtat * (uv + cosTheta * n);
        Vec3 rOutParallel = -std::sqrt(std::fabs(1.0 - rOutPerp.LengthSquared())) * n;
        return rOutPerp + rOutParallel;
    }



    // Ray
    class Ray
    {
    public:
        Ray() {}

        Ray(const Point3 &origin, const Vec3 &direction, Float time) : o(origin), d(direction), tm(time) {}
        Ray(const Point3 &origin, const Vec3 &direction) : Ray(origin, direction, 0) {}

        const Point3 &Orig() const { return o; }
        const Vec3 &Dir() const { return d; }
        Float Time() const { return tm; }

        Point3 At(Float t) const
        {
            return o + t * d;
        }

    private:
        Point3 o;
        Vec3 d;
        Float tm;
    };

    class Interval
    {
    public:
        Float min, max;
        Interval() : min(+INF), max(-INF) {}
        Interval(Float min, Float max) : min(min), max(max) {}
        Interval(const Interval &a, const Interval &b)
        {
            // Create the interval tightly enclosing the two input intervals.
            min = a.min <= b.min ? a.min : b.min;
            max = a.max >= b.max ? a.max : b.max;
        }

        Float Size() const { return max - min; }

        bool Contains(Float x) const { return min <= x && x <= max; }
        bool Surrounds(Float x) const { return min < x && x < max; }

        Float Clamp(Float x) const
        {
            if (x < min)
                return min;
            if (x > max)
                return max;
            return x;
        }

        Interval Expand(Float delta) const
        {
            auto padding = delta / 2;
            return Interval(min - padding, max + padding);
        }

        static const Interval empty, universe;
    };

    const Interval Interval::empty = Interval(+INF, -INF);
    const Interval Interval::universe = Interval(-INF, +INF);

    Interval operator+(const Interval &ival, Float displacement)
    {
        return Interval(ival.min + displacement, ival.max + displacement);
    }
    Interval operator+(Float displacement, const Interval &ival)
    {
        return ival + displacement;
    }
}

#endif