#include "core/interval.h"

const Interval Interval::empty =    Interval(+inf, -inf);
const Interval Interval::universe = Interval(-inf, +inf);

Interval::Interval(const Interval &a, const Interval &b)
{
    // Create the interval tightly enclosing the two input intervals.
    min = a.min <= b.min ? a.min : b.min;
    max = a.max >= b.max ? a.max : b.max;
}

Interval operator+(const Interval &ival, F32 displacement)
{
    return Interval(ival.min + displacement, ival.max + displacement);
}

Interval operator+(F32 displacement, const Interval &ival)
{
    return ival + displacement;
}