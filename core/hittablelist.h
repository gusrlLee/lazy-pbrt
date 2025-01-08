#pragma once

#include "core/ray.h"
#include "core/hitrecord.h"
#include "core/hittable.h"

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

private:
    AABB bbox;
};