#ifndef NANO_GLOBAL_ILLUMINATION_DATA_HPP
#define NANO_GLOBAL_ILLUMINATION_DATA_HPP

#include <iostream>
#include <cmath>
#include <stdio.h>
#include <numbers>
#include <algorithm>

namespace nanogi
{
    typedef int Int;
#if 1
    typedef double Float;
#else
    typedef float Float;

#endif
    const Float Inf = std::numeric_limits<Float>::infinity();
    const Float Pi = 3.1415926535897932385;
    const Float Eps = std::numeric_limits<Float>::epsilon();

    class Float2
    {
    public:
        // elements
        Float e[2];

        // constructions
        Float2() : e{0, 0} {}
        Float2(Float v) : e{v, v} {}
        Float2(Float v0, Float v1) : e{v0, v1} {}

        Float x() const { return e[0]; }
        Float y() const { return e[1]; }

        Float2 operator-() const { return Float2(-e[0], -e[1]); }
        Float operator[](Int i) const { return e[i]; }
        Float &operator[](Int i) { return e[i]; }

        Float2 &operator+=(const Float2 &v)
        {
            e[0] += v.e[0];
            e[1] += v.e[1];
            return *this;
        }

        Float2 &operator*=(const Float2 &v)
        {
            e[0] *= v.e[0];
            e[1] *= v.e[1];
            return *this;
        }

        Float2 &operator/=(Float t)
        {
            return *this *= 1 / t;
        }

        Float LenSq() const
        {
            return e[0] * e[0] + e[1] * e[1];
        }

        Float Len() const
        {
            return std::sqrt(LenSq());
        }

        bool IsNearZero() const
        {
            // Return true if the vector is close to zero in all dimensions.
            return (std::fabs(e[0]) < Eps) && (std::fabs(e[1]) < Eps);
        }
    }; // Float2

    inline Float2 operator+(const Float2 &u, const Float2 &v)
    {
        return Float2(u.e[0] + v.e[0], u.e[1] + v.e[1]);
    }

    inline Float2 operator-(const Float2 &u, const Float2 &v)
    {
        return Float2(u.e[0] - v.e[0], u.e[1] - v.e[1]);
    }

    inline Float2 operator*(const Float2 &u, const Float2 &v)
    {
        return Float2(u.e[0] * v.e[0], u.e[1] * v.e[1]);
    }

    inline Float2 operator*(Float t, const Float2 &v)
    {
        return Float2(t * v.e[0], t * v.e[1]);
    }

    inline Float2 operator*(const Float2 &v, Float t)
    {
        return t * v;
    }

    inline Float2 operator/(const Float2 &v, Float t)
    {
        return (1 / t) * v;
    }

    using Vec2 = Float2;
    using Point2 = Float2;

    /*
        Float3
    */
    class Float3
    {
    public:
        // elements
        Float e[3];

        // constructions
        Float3() : e{0, 0, 0} {}
        Float3(Float v) : e{v, v, v} {}
        Float3(Float v0, Float v1, Float v2) : e{v0, v1, v2} {}

        // for vector
        Float x() const { return e[0]; }
        Float y() const { return e[1]; }
        Float z() const { return e[2]; }

        // for color
        Float r() const { return e[0]; }
        Float g() const { return e[1]; }
        Float b() const { return e[2]; }

        Int rByte() const { return Int(256 * std::clamp(e[0], (Float)0.0000, (Float)0.9999)); }
        Int gByte() const { return Int(256 * std::clamp(e[1], (Float)0.0000, (Float)0.9999)); }
        Int bByte() const { return Int(256 * std::clamp(e[2], (Float)0.0000, (Float)0.9999)); }

        Float3 operator-() const { return Float3(-e[0], -e[1], -e[2]); }
        Float operator[](Int i) const { return e[i]; }
        Float &operator[](Int i) { return e[i]; }

        Float3 &operator+=(const Float3 &v)
        {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        Float3 &operator*=(const Float3 &v)
        {
            e[0] *= v.e[0];
            e[1] *= v.e[1];
            e[2] *= v.e[2];
            return *this;
        }

        Float3 &operator/=(Float t)
        {
            return *this *= 1 / t;
        }

        Float LenSq() const
        {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }

        Float Len() const
        {
            return std::sqrt(LenSq());
        }

        bool IsNearZero() const
        {
            return (std::fabs(e[0]) < Eps) && (std::fabs(e[1]) < Eps) && (std::fabs(e[2]) < Eps);
        }
    };

    inline Float3 operator+(const Float3 &u, const Float3 &v)
    {
        return Float3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
    }

    inline Float3 operator-(const Float3 &u, const Float3 &v)
    {
        return Float3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
    }

    inline Float3 operator*(const Float3 &u, const Float3 &v)
    {
        return Float3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
    }

    inline Float3 operator*(double t, const Float3 &v)
    {
        return Float3(t * v.e[0], t * v.e[1], t * v.e[2]);
    }

    inline Float3 operator*(const Float3 &v, Float t)
    {
        return t * v;
    }

    inline Float3 operator/(const Float3 &v, Float t)
    {
        return (1 / t) * v;
    }

    inline Float Dot(const Float3 &u, const Float3 &v)
    {
        return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
    }

    inline Float3 Cross(const Float3 &u, const Float3 &v)
    {
        return Float3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                      u.e[2] * v.e[0] - u.e[0] * v.e[2],
                      u.e[0] * v.e[1] - u.e[1] * v.e[0]);
    }

    inline Float3 Normalize(const Float3 &v)
    {
        return v / v.Len();
    }

    using Vec3 = Float3;
    using Point3 = Float3;
    using Color = Float3;
    using Vertex = Float3;
    using Normal = Float3;

    class Onb // orthonormal basis
    {
    public:
        Onb(const Vec3 &n)
        {
            axis[2] = Normalize(n);
            Vec3 a = (std::fabs(axis[2].x()) > 0.9) ? Vec3(0, 1, 0) : Vec3(1, 0, 0);
            axis[1] = Normalize(Cross(axis[2], a));
            axis[0] = Cross(axis[2], axis[1]);
        }

        const Vec3 &u() const { return axis[0]; }
        const Vec3 &v() const { return axis[1]; }
        const Vec3 &w() const { return axis[2]; }

        Vec3 Transform(const Vec3 &v) const
        {
            // Transform from basis coordinates to local space.
            return (v[0] * axis[0]) + (v[1] * axis[1]) + (v[2] * axis[2]);
        }

    private:
        Vec3 axis[3];
    };

    class Ray
    {
    public:
        Ray() {}
        Ray(Point3 o, Float3 d) : o(o), d(d) {}

        const Point3 &Orig() const { return o; }
        const Vec3 &Dir() const { return d; }
        const Point3 At(Float t) const { return o + d * t; }

    private:
        Point3 o;
        Vec3 d;
    };

    class Material;
    class Pdf;
    // hRec : HitRecord
    // sRec : ScatterRecord
    class Payload
    {
    public:
        struct HitRecord
        {
            Float t;                       // ray tFar value
            Point3 p;                      // intersection point
            Normal n;                      // normal vector
            Int gID;                       // geometry ID
            Int pID;                       // primitive ID
            std::shared_ptr<Material> mat; // material of hit primitive
        };

        struct ScatterRecord
        {
            bool isSkipPdf;            // to handling specular light
            std::shared_ptr<Pdf> pPdf; // pdf value pointer
            Ray nextRay;               // next ray == wo
            Float pdf;
        };

        HitRecord hRec;
        ScatterRecord sRec;
    };
} // namespace nanogi

#endif
