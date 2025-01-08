#pragma once 

#include "core/types.h"
#include "math/vec3.h"

class Interval
{
public:
    F32 min, max;
    Interval() : min(+inf), max(-inf) {}
    Interval(F32 min, F32 max) : min(min), max(max) {}
    Interval(const Interval &a, const Interval &b);

    F32 Size() const { return max - min; }

    bool Contains(F32 x) const { return min <= x && x <= max; }
    bool Surrounds(F32 x) const { return min < x && x < max; }

    F32 Clamp(F32 x) const 
    {
        if (x < min)  return min;
        if (x > max) return max;
        return x;
    }

    Interval Expand(F32 delta) const 
    {
        auto padding = delta / 2;
        return Interval(min - padding, max + padding);
    }

    static const Interval empty, universe;
};

