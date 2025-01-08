#pragma once


#include "core/types.h"
#include "core/ray.h"
#include "core/interval.h"
#include "math/aabb.h"

#include "core/hitrecord.h"


class HitTable
{
public:
    virtual ~HitTable() = default;
    virtual bool Hit(const Ray &r, Interval t, HitRecord &rec) const = 0;
    virtual AABB BBox() const = 0;
};