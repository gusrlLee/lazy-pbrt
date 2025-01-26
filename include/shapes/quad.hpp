#ifndef GI_SHAPES_QUAD_HPP
#define GI_SHAPES_QUAD_HPP

#include "core/hittablelist.hpp"

namespace gi
{
    class Quad : public Hittable
    {
    public:
        Quad(const Point3 &Q, const Vec3 &u, const Vec3 v, SPtr<Material> mat) : Q(Q), u(u), v(v), mat(mat)
        {
            auto n = Cross(u, v);
            this->N = Normalize(n);
            D = Dot(N, Q);
            W = n / Dot(n, n);

            area = n.Length();

            SetBoundingBox();
        }

        virtual void SetBoundingBox()
        {
            auto bboxDiagonal1 = AABB(Q, Q + u + v);
            auto bboxDiagonal2 = AABB(Q + u, Q + v);

            bbox = AABB(bboxDiagonal1, bboxDiagonal2);
        }

        AABB BBox() const override { return bbox; }

        bool Hit(const Ray &r, Interval t, HitRecord &rec) const override
        {
            auto denom = Dot(N, r.Dir());
            if (std::fabs(denom) < 1e-8)
                return false;

            // intersection Point
            auto tValue = (D - Dot(N, r.Orig())) / denom;
            if (!t.Contains(tValue))
                return false;

            auto ist = r.At(tValue);
            Vec3 planerHitPtVector = ist - Q;
            auto alpha = Dot(W, Cross(planerHitPtVector, v));
            auto beta = Dot(W, Cross(u, planerHitPtVector));

            if (!IsInterior(alpha, beta, rec))
                return false;

            rec.t = tValue;
            rec.P = ist;
            rec.mat = mat;
            rec.SetFaceNormal(r, N);
            return true;
        }
        virtual bool IsInterior(Float a, Float b, HitRecord &rec) const
        {
            Interval unitInterval = Interval(0, 1);
            if (!unitInterval.Contains(a) || !unitInterval.Contains(b))
                return false;

            rec.u = a;
            rec.v = b;
            return true;
        }

        Float PdfValue(const Point3 &origin, const Vec3 &dir) const override
        {
            HitRecord rec;
            if (!this->Hit(Ray(origin, dir), Interval(0.001, INF), rec))
                return 0;

            auto distSquared = rec.t * rec.t * dir.LengthSquared();
            auto cosine = std::fabs(Dot(dir, rec.N) / dir.Length());

            return distSquared / (cosine * area);
        }
        Vec3 Random(const Point3 &origin) const override
        {
            auto p = Q + (RNG::Get1D() * u) + (RNG::Get1D() * v);
            return p - origin;
        }

    private:
        Point3 Q;
        Vec3 u, v;
        SPtr<Material> mat;
        Vec3 W;
        AABB bbox;
        Vec3 N;
        Float D;
        Float area;
    };

    inline SPtr<HittableList> Box(const Point3 &a, const Point3 &b, SPtr<Material> mat)
    {
        auto sides = MakeShared<HittableList>();

        auto min = Point3(std::fmin(a.x(), b.x()), std::fmin(a.y(), b.y()), std::fmin(a.z(), b.z()));
        auto max = Point3(std::fmax(a.x(), b.x()), std::fmax(a.y(), b.y()), std::fmax(a.z(), b.z()));

        auto dx = Vec3(max.x() - min.x(), 0, 0);
        auto dy = Vec3(0, max.y() - min.y(), 0);
        auto dz = Vec3(0, 0, max.z() - min.z());

        sides->Add(MakeShared<Quad>(Point3(min.x(), min.y(), max.z()), dx, dy, mat));  // front
        sides->Add(MakeShared<Quad>(Point3(max.x(), min.y(), max.z()), -dz, dy, mat)); // right
        sides->Add(MakeShared<Quad>(Point3(max.x(), min.y(), min.z()), -dx, dy, mat)); // back
        sides->Add(MakeShared<Quad>(Point3(min.x(), min.y(), min.z()), dz, dy, mat));  // left
        sides->Add(MakeShared<Quad>(Point3(min.x(), max.y(), max.z()), dx, -dz, mat)); // top
        sides->Add(MakeShared<Quad>(Point3(min.x(), min.y(), min.z()), dx, dz, mat));  // bottom

        return sides;
    }

}
#endif