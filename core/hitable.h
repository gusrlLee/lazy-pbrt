#pragma once

#include "core/types.h"
#include "core/ray.h"
#include "core/interval.h"
#include "core/hitrecord.h"

class HitTable
{
public:
    virtual ~HitTable() = default;
    virtual bool Hit(const Ray &r, Interval t, HitRecord &rec) const = 0;
};