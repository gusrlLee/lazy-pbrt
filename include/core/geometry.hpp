#ifndef GI_CORE_GEOMETRY_HPP
#define GI_CORE_GEOMETRY_HPP

#include "core/types.hpp"

namespace gi
{
    template <typename T>
    struct Vec2
    {
        T x, y;
    };

    using Vec2i = Vec2<Int>;
    using Vec2f = Vec2<Float>;
    using Point2i = Vec2<Int>;
    using Point2f = Vec2<Float>;

    template <typename T>
    inline Vec2<T> Add(const Vec2<T> &u, const Vec2<T> &v)
    {
        Vec2<T> res = {u.x + v.x, u.y + v.y};
        return res;
    }

    template <typename T>
    inline Vec2<T> Sub(const Vec2<T> &u, const Vec2<T> &v)
    {
        Vec2<T> res = {u.x - v.x, u.y - v.y};
        return res;
    }

    template <typename T>
    inline Vec2<T> Mul(const Vec2<T> &u, const Vec2<T> &v)
    {
        Vec2<T> res = {u.x * v.x, u.y * v.y};
        return res;
    }

    template <typename T>
    inline Vec2<T> Mul(const Vec2<T> &u, const T &v)
    {
        Vec2<T> res = {u.x * v, u.y * v};
        return res;
    }

    template <typename T>
    inline Vec2<T> Div(const Vec2<T> &u, const Vec2<T> &v)
    {
        Vec2<T> res = {u.x / v.x, u.y / v.y};
        return res;
    }

    template <typename T>
    inline Vec2<T> Div(const Vec2<T> &u, const T &v)
    {
        Vec2<T> res = {u.x / v, u.y / v};
        return res;
    }

    template <typename T>
    struct Vec3
    {
        T x, y, z;
    };

    using Vec3i = Vec3<Int>;
    using Vec3f = Vec3<Float>;
    using Vertex = Vec3<Float>;
    using Point3i = Vec3<Int>;
    using Point3f = Vec3<Float>;
    using Normal = Vec3<Float>;

    template <typename T>
    inline Vec3<T> Add(const Vec3<T> &u, const Vec3<T> &v)
    {
        Vec3<T> res = {u.x + v.x, u.y + v.y, u.z + v.z};
        return res;
    }

    template <typename T>
    inline Vec3<T> Sub(const Vec3<T> &u, const Vec3<T> &v)
    {
        Vec3<T> res = {u.x - v.x, u.y - v.y, u.z - v.z};
        return res;
    }

    template <typename T>
    inline Vec3<T> Mul(const Vec3<T> &u, const Vec3<T> &v)
    {
        Vec3<T> res = {u.x * v.x, u.y * v.y, u.z * v.z};
        return res;
    }

    template <typename T>
    inline Vec3<T> Mul(const Vec3<T> &u, const T &v)
    {
        Vec3<T> res = {u.x * v, u.y * v, u.z * v};
        return res;
    }

    template <typename T>
    inline Vec3<T> Div(const Vec3<T> &u, const Vec3<T> &v)
    {
        Vec3<T> res = {u.x / v.x, u.y / v.y, u.z / v.z};
        return res;
    }

    template <typename T>
    inline Vec3<T> Div(const Vec3<T> &u, const T &v)
    {
        Vec3<T> res = {u.x / v, u.y / v, u.z / v};
        return res;
    }

    template <typename T>
    inline Float Dot(const Vec3<T> &u, const Vec3<T> &v)
    {
        return u.x * v.x + u.y * v.y + u.z * v.z;
    }

    template <typename T>
    inline Vec3<T> Cross(const Vec3<T> &u, const Vec3<T> &v)
    {
        Vec3<T> res = {
            u.y * v.z - u.z * v.y,
            u.z * v.x - u.x * v.z,
            u.x * v.y - u.y * v.x
        };
        return res;
    }

    template <typename T> 
    inline Float Len(const Vec3<T> &v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    template <typename T> 
    inline Float LenSq(const Vec3<T> &v)
    {
        return v.x * v.x + v.y * v.y + v.z * v.z;
    }

    template <typename T> 
    inline Vec3<T> Normalize(const Vec3<T> &v)
    {
        return Div(v, Len(v));
    }

    template <typename T> 
    inline Vec3<T> Reverse(const Vec3<T> &v)
    {
        Vec3<T> res = {-v.x, -v.y, -v.z};
        return res;
    }

    typedef struct Color
    {
        Float r, g, b;
    } Color;

    typedef struct Ray
    {
        Point3f o;
        Vec3f d;
    } Ray;

    typedef struct Triangle
    {
        Vertex v0, v1, v2;
        UInt matIdx;
    } Triangle;
}

#endif