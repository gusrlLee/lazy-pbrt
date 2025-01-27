#ifndef GI_CORE_HITTABLE_LIST_HPP
#define GI_CORE_HITTABLE_LIST_HPP

#include "core/hittable.hpp"
#include "core/rng.hpp"

namespace gi
{
    class HittableList : public Hittable
    {
    public:
        std::vector<std::shared_ptr<Hittable>> objects;

        HittableList() {}
        HittableList(std::shared_ptr<Hittable> object) { Add(object); }

        void Clear() { objects.clear(); }
        void Add(std::shared_ptr<Hittable> object)
        {
            objects.push_back(object);
            bbox = AABB(bbox, object->BBox());
        }

        Int Size() { return objects.size(); }

        Bool Hit(const Ray &r, Interval t, HitRecord &rec) const override
        {
            HitRecord tempRec;
            bool isHitAnything = false;
            auto closestSoFar = t.max;

            for (const auto &object : objects)
            {
                // execute to intersection test about current primitive
                if (object->Hit(r, Interval(t.min, closestSoFar), tempRec))
                {
                    isHitAnything = true;
                    closestSoFar = tempRec.t;
                    rec = tempRec;
                }
            }

            return isHitAnything;
        }

        AABB BBox() const override { return bbox; }

        Float PdfValue(const Point3 &origin, const Vec3 &dir) const override
        {
            auto weight = 1.0 / objects.size();
            auto sum = 0.0;

            for (const auto &object : objects)
            {
                sum += weight * object->PdfValue(origin, dir);
            }

            return sum;
        }

        Vec3 Random(const Point3 &origin) const override
        {
            auto size = Int(objects.size());
            return objects[RNG::Get1D(0, size - 2)]->Random(origin); // due to Random::ValueI32 function is Value(min, max + 1). So we have to write size + 1 to avoid segementation fault.
        }

    private:
        AABB bbox;
    };

}
#endif
