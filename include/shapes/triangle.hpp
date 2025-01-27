#ifndef GI_SHAPES_TRIANGLE_HPP
#define GI_SHAPES_TRIANGLE_HPP

#include "core/hittable.hpp"
#include "core/geometry.hpp"
#include "core/aabb.hpp"
#include "core/rng.hpp"

namespace gi
{
    class Triangle : public Hittable
    {
    public:
        Vertex vertex[3];

        Triangle() {}
        Triangle(Point3 v0, Point3 v1, Point3 v2, SPtr<Material> mat) : vertex{v0, v1, v2}, mat(mat)
        {
            bbox = AABB(v0, v1, v2);
            N = Cross(edge1(), edge2());
            area = N.Length() / 2;
        }

        // vertex
        Vertex &operator[](Int i) { return vertex[i]; }
        Vertex v0() const { return vertex[0]; }
        Vertex v1() const { return vertex[1]; }
        Vertex v2() const { return vertex[2]; }

        // edge
        Vec3 edge1() const { return vertex[1] - vertex[0]; }
        Vec3 edge2() const { return vertex[2] - vertex[0]; }

        // Normal
        Vec3 Normal() const { return Normalize(Cross(edge1(), edge2())); }

        AABB BBox() const override { return bbox; }

        // Möller–Trumbore intersection algorithm
        // link : https://github.com/jbikker/bvh_article/blob/main/basics.cpp
        Bool Hit(const Ray &r, Interval t, HitRecord &rec) const override
        {
            Vec3 e1 = this->edge1();
            Vec3 e2 = this->edge2();

            Vec3 h = Cross(r.Dir(), e2);
            Float a = Dot(e1, h);
            // Ray parallel to triangle
            if (a > -EPS && a < EPS)
                return false;

            Float f = 1 / a;
            Vec3 s = r.Orig() - this->v0();
            Float u = f * Dot(s, h);
            if (u < 0 || u > 1)
                return;

            Vec3 q = Cross(s, e1);
            Float v = f * Dot(r.Dir(), q);
            if (v < 0 || u + v > 1)
                return;

            // success to intersection test of triangle
            Float tValue = f * Dot(e2, q);
            rec.t = tValue;
            rec.P = r.At(tValue);
            Vec3 outNormal = this->Normal();
            rec.SetFaceNormal(r, outNormal);

            rec.mat = mat;
            rec.u = u;
            rec.v = v;

            return true;
        }

        Float PdfValue(const Point3 &origin, const Vec3 &dir) const override
        {
            HitRecord rec;
            if (!this->Hit(Ray(origin, dir), Interval(0.001, INF), rec))
                return 0;

            auto distSq = rec.t * rec.t * dir.LengthSquared();
            auto cosine = std::fabs(Dot(dir, rec.N) / dir.Length());        
            return distSq / (cosine * area);
        }

        Vec3 Random(const Point3 &origin) const override
        {
            Float u = RNG::Get1D(0, 1);
            Float v = RNG::Get1D(0, 1);
            Vec3 p = Vec3(std::sqrtf(u), u * v, (1.0f - u - v));
            return p - origin;
        }

    private:
        SPtr<Material> mat;
        AABB bbox;
        Vec3 W, N;
        Float area;
    };
}

#endif