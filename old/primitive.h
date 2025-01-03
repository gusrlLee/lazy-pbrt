#pragma once 

#include "ray.h"
#include "types.h"
#include "hit_record.h"
#include "interval.h"

class Primitive
{
    public:
        virtual ~Primitive() = default;
        virtual bool hit(const Ray& r, Interval r_t, hit_record& rec) const = 0;
};
