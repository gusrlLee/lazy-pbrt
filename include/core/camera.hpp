#ifndef GI_CORE_CAMERA_HPP
#define GI_CORE_CAMERA_HPP

#include "core/geometry.hpp"

namespace gi
{
    class Camera
    {
    public:
        Camera() : width(400), height(400), from{5.0001f, 5.0001f, -3.0f}, to{0, 0, 0}, up(0, 1, 0), fov(90.0f) { SetUp(); }
        Camera(const Int w, const Int h, const Vec3f &from, const Vec3f &to, const Vec3f &up, Float fov) : width(w), height(h), from(from), to(to), up(up), fov(fov) { SetUp(); }

        Int GetWidth() { return width; }
        Int GetHeight() { return height; }

        Ray GenRay(Point2i &px, Vec2f offset)
        {
            auto px_sample = Add(px00, Add((Mul(pxdu, (Float(px.x) + offset.x))), Mul(pxdv, (px.y + offset.y))));
            auto o = from;
            auto d = Normalize(Sub(px_sample, o));
            Ray r = {o, d};
            return r;
        }

    public:
        Int width, height;
        Vec3f from;
        Vec3f to;
        Vec3f up;
        Float fov;
        Vec3f w, u, v;

        Vec3f pxdu, pxdv;
        Point3f px00;

    private:
        void SetUp()
        {
#if 1 
            // for cornell box
            from = {278.0f, 278.0f, -800.0f};
            to = {278.0f, 278.0f, 0.0f};
            up = {0.0f, 1.0f, 0.0f};
            fov = 40.0f;
#endif

            auto fLen = Len(Sub(from, to));
            // Camera settnig
            auto theta = fov * PI / 180.0f;
            auto h = std::tan(theta / 2);

            auto vportH = 2 * h * fLen;
            auto vportW = vportH * (Float(width) / Float(height));

            // Setting camera unit basis
            w = Normalize(Sub(from, to));
            u = Normalize(Cross(up, w));
            v = Cross(w, u);

            auto vportU = Mul(u, vportW);
            auto vportV = Mul(Reverse(v), vportH);

            pxdu = Div(vportU, Float(width));
            pxdv = Div(vportV, Float(height));

            auto vportUpper = Sub(from, Mul(w, fLen));
            vportUpper = Sub(vportUpper, Div(vportU, 2.0f));
            vportUpper = Sub(vportUpper, Div(vportV, 2.0f));

            px00 = Add(vportUpper, Mul(Add(pxdu, pxdv), 0.5f));
        }
    };
}

#endif