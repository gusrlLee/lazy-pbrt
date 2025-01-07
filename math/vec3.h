#pragma once

#include "core/types.h"

#include <cmath>
#include <iostream>

class Vec3
{
public:
    F32 e[3];

    Vec3() : e{0, 0, 0} {}
    Vec3(F32 e0, F32 e1, F32 e2) : e{e0, e1, e2} {}

    F32 x() const { return e[0]; }
    F32 y() const { return e[1]; }
    F32 z() const { return e[2]; }

    Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    F32 operator[](int i) const { return e[i]; }
    F32 &operator[](int i) { return e[i]; }

    Vec3 &operator+=(const Vec3 &v)
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vec3 &operator*=(F32 t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vec3 &operator/=(F32 t)
    {
        return *this *= 1 / t;
    }

    F32 Length() const
    {
        return std::sqrt(LengthSqaured());
    }

    F32 LengthSqaured() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    bool IsNearZero() const
    {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }
};

// point3 is just an alias for Vec3, but useful for geometric clarity in the code.
using Point3 = Vec3;

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

inline Vec3 operator*(F32 t, const Vec3 &v)
{
    return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator*(const Vec3 &v, F32 t)
{
    return t * v;
}

inline Vec3 operator/(const Vec3 &v, F32 t)
{
    return (1 / t) * v;
}

inline F32 Dot(const Vec3 &u, const Vec3 &v)
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

inline Vec3 Refract(const Vec3 &uv, const Vec3 &n, F32 etaiOverEtat)
{
    auto cosTheta = std::fmin(Dot(-uv, n), 1.0);
    Vec3 rOutPerp = etaiOverEtat * (uv + cosTheta * n);
    Vec3 rOutParallel = -std::sqrt(std::fabs(1.0 - rOutPerp.LengthSqaured())) * n;
    return rOutPerp + rOutParallel;
}
