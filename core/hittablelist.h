#pragma once

#include "core/ray.h"
#include "core/hitrecord.h"
#include "core/hittable.h"
#include "core/random.h"

#include "math/aabb.h"

class HitTableList : public HitTable
{
public:
    std::vector<std::shared_ptr<HitTable>> objects;

    HitTableList() {}
    HitTableList(std::shared_ptr<HitTable> object) { Add(object); }

    void Clear() { objects.clear(); }
    void Add(std::shared_ptr<HitTable> object)
    {
        objects.push_back(object);
        bbox = AABB(bbox, object->BBox());
    }
    bool Hit(const Ray &r, Interval t, HitRecord &rec) const override;
    AABB BBox() const override { return bbox; }

    F32 PdfValue(const Point3 &origin, const Vec3 &dir) const override;
    Vec3 Random(const Point3 &origin) const override
    {
        auto size = I32(objects.size());
        return objects[Random::ValueI32(0, size - 2)]->Random(origin); // due to Random::ValueI32 function is Value(min, max + 1). So we have to write size + 1 to avoid segementation fault.
    }

private:
    AABB bbox;
};