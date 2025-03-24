#ifndef NANO_GLOBAL_ILLUMINATION_CAMERA_HPP
#define NANO_GLOBAL_ILLUMINATION_CAMERA_HPP

#include "nanogi/data.hpp"

namespace nanogi
{
    class Camera
    {
    public:
        Camera() : width(600), height(600), from(278.0f, 278.0f, -800.0f), to(278.0f, 278.0f, 0), up(0, 1, 0), fov(40.0f) { SetUp(); }
        Camera(const Int w, const Int h, const Vec3 &from, const Vec3 &to, const Vec3 &up, Float fov) : width(w), height(h), from(from), to(to), up(up), fov(fov) { SetUp(); }

        Int GetWidth() { return width; }
        Int GetHeight() { return height; }

        Ray GenRay(Point2 &px, Point2 &offset)
        {
            auto pxSample = px00 + pxdu * (px.x() + offset.x()) + pxdv * (px.y() + offset.y());
            auto o = from;
            auto d = Normalize(pxSample - o);
            Ray r = Ray(o, d);
            return r;
        }

    public:
        Int width, height;
        Vec3 from;
        Vec3 to;
        Vec3 up;
        Float fov;
        Vec3 w, u, v;

        Vec3 pxdu, pxdv;
        Point3 px00;

    private:
        void SetUp()
        {
            auto fLen = (from - to).Len();
            // Camera settnig
            auto theta = fov * Pi / 180;
            auto h = std::tan(theta / 2);

            auto vportH = 2 * h * fLen;
            auto vportW = vportH * (Float(width) / Float(height));

            // Setting camera unit basis
            w = Normalize(from - to);
            u = Normalize(Cross(up, w));
            v = Cross(w, u);

            auto vportU = u * vportW;
            auto vportV = -v * vportH;

            pxdu = vportU / Float(width);
            pxdv = vportV / Float(height);

            auto vportUpperLeft = from - (fLen * w) - vportU / 2 - vportV / 2;
            px00 = vportUpperLeft + 0.5 * (pxdu + pxdv);
        }
    };
}

#endif