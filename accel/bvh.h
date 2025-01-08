#pragma once

#include "math/aabb.h"
#include "core/types.h"
#include "core/hittable.h"
#include "core/hittablelist.h"
#include "core/random.h"

#include <algorithm>

class BVHNode : public HitTable
{
public:
    BVHNode(HitTableList list);
    BVHNode(std::vector<Sptr<HitTable>> &objects, Size start, Size end);

    bool Hit(const Ray &r, Interval t, HitRecord &rec) const override;
    AABB BBox() const override { return bbox; }

private:
    Sptr<HitTable> left;
    Sptr<HitTable> right;
    AABB bbox;

    static bool BoxCompare(const Sptr<HitTable> a, const Sptr<HitTable> b, I32 idx);
    static bool BoxCompareX(const Sptr<HitTable> a, const Sptr<HitTable> b);
    static bool BoxCompareY(const Sptr<HitTable> a, const Sptr<HitTable> b);
    static bool BoxCompareZ(const Sptr<HitTable> a, const Sptr<HitTable> b);
};