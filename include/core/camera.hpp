#ifndef GI_CORE_CAMERA_HPP
#define GI_CORE_CAMERA_HPP

#include "core/geometry.hpp"

namespace gi
{
    class Camera
    {
    public:
        Camera() : width(720), height(480), from{0.0001f, 0.0001f, -3.0f}, to{0, 0, 0}, up(0, 1, 0), fov(90.0f) { SetUp(); }
        Camera(const Int w, const Int h, const Vec3f &from, const Vec3f &to, const Vec3f &up, Float fov) : width(w), height(h), from(from), to(to), up(up), fov(fov) { SetUp(); }

        Int GetWidth() { return width; }
        Int GetHeight() { return height; }

        Ray GenRay(Point2i &px)
        {
            auto px_sample = Add(px00, Add((Mul(pxdu, (px.x + 0.5f))), Mul(pxdv, (px.y + 0.5f))));
            auto o = from;
            auto d = Sub(px_sample, o);
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