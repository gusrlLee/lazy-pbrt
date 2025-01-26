#ifndef GI_CORE_HITTABLE_HPP
#define GI_CORE_HITTABLE_HPP

#include "core/hitrecord.hpp"
#include "core/aabb.hpp"
#include "core/geometry.hpp"

namespace gi
{
    class Hittable
    {
    public:
        virtual ~Hittable() = default;
        virtual Bool Hit(const Ray &r, Interval t, HitRecord &rec) const = 0;
        virtual AABB BBox() const = 0;

        virtual Float PdfValue(const Point3 &origin, const Vec3 &dir) const { return 0.0f; }
        virtual Vec3 Random(const Point3 &origin) const
        {
            return Vec3(1, 0, 0);
        }
    };

    class Translate : public Hittable
    {
    public:
        Translate(SPtr<Hittable> object, const Vec3 &offset) : object(object), offset(offset)
        {
            bbox = object->BBox() + offset;
        }

        bool Hit(const Ray &r, Interval t, HitRecord &rec) const override
        {
            Ray offsetRay(r.Orig() - offset, r.Dir(), r.Time());

            if (!object->Hit(offsetRay, t, rec))
                return false;

            rec.P += offset;
            return true;
        }

        AABB BBox() const override { return bbox; }

    private:
        SPtr<Hittable> object;
        Vec3 offset;
        AABB bbox;
    };

    class RotateY : public Hittable
    {
    public:
        RotateY(SPtr<Hittable> object, Float angle) : object(object)
        {
            auto radians = DegreeToRadians(angle);
            sinTheta = std::sin(radians);
            cosTheta = std::cos(radians);
            bbox = object->BBox();

            Point3 min(INF, INF, INF);
            Point3 max(-INF, -INF, -INF);

            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < 2; j++)
                {
                    for (int k = 0; k < 2; k++)
                    {
                        auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
                        auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
                        auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

                        auto newx = cosTheta * x + sinTheta * z;
                        auto newz = -sinTheta * x + cosTheta * z;

                        Vec3 tester(newx, y, newz);

                        for (int c = 0; c < 3; c++)
                        {
                            min[c] = std::fmin(min[c], tester[c]);
                            max[c] = std::fmax(max[c], tester[c]);
                        }
                    }
                }
            }

            bbox = AABB(min, max);
        }

        bool Hit(const Ray &r, Interval t, HitRecord &rec) const override
        {
            // Transform the ray from world space to object space.
            auto origin = Point3(
                (cosTheta * r.Orig().x()) - (sinTheta * r.Orig().z()),
                r.Orig().y(),
                (sinTheta * r.Orig().x()) + (cosTheta * r.Orig().z()));

            auto direction = Vec3(
                (cosTheta * r.Dir().x()) - (sinTheta * r.Dir().z()),
                r.Dir().y(),
                (sinTheta * r.Dir().x()) + (cosTheta * r.Dir().z()));

            Ray rotated_r(origin, direction, r.Time());

            // Determine whether an intersection exists in object space (and if so, where).

            if (!object->Hit(rotated_r, t, rec))
                return false;

            // Transform the intersection from object space back to world space.

            rec.P = Point3(
                (cosTheta * rec.P.x()) + (sinTheta * rec.P.z()),
                rec.P.y(),
                (-sinTheta * rec.P.x()) + (cosTheta * rec.P.z()));

            rec.N = Vec3(
                (cosTheta * rec.N.x()) + (sinTheta * rec.N.z()),
                rec.N.y(),
                (-sinTheta * rec.N.x()) + (cosTheta * rec.N.z()));

            return true;
        }

        AABB BBox() const override { return bbox; }

    private:
        SPtr<Hittable> object;
        Float sinTheta;
        Float cosTheta;
        AABB bbox;
    };
}

#endif