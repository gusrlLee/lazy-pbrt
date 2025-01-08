#include "core/interval.h"

const Interval Interval::empty =    Interval(+inf, -inf);
const Interval Interval::universe = Interval(-inf, +inf);

Interval::Interval(const Interval &a, const Interval &b)
{
    // Create the interval tightly enclosing the two input intervals.
    min = a.min <= b.min ? a.min : b.min;
    max = a.max >= b.max ? a.max : b.max;
}