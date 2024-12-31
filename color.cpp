#include "color.h"
#include "interval.h"

inline f64 linear_to_gamma(f64 linear_compontent)
{
    if (linear_compontent > 0)
        return std::sqrt(linear_compontent);

    return 0;
}

void write_color(std::ostream &out, Color pixel_color)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const Interval intensity(0.000, 0.999);
    i32 rbyte = i32(256 * intensity.clamp(r));
    i32 gbyte = i32(256 * intensity.clamp(g));
    i32 bbyte = i32(256 * intensity.clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n'; 
}
