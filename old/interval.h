#pragma once 

#include "types.h"
#include "vec3.h"

class Interval
{
public:
    f64 min, max;
    Interval() : min(+inf), max(-inf) {}
    Interval(f64 min, f64 max) : min(min), max(max) {}

    f64 size() const { return max - min; }

    bool contains(f64 x) const { return min <= x && x <= max; }
    bool surrounds(f64 x) const { return min < x && x < max; }

    f64 clamp(f64 x) const 
    {
        if (x < min)  return min;
        if (x > max) return max;
        return x;
    }

    Interval expand(f64 delta) const 
    {
        auto padding = delta / 2;
        return Interval(min - padding, max + padding);
    }

    static const Interval empty, universe;
};

