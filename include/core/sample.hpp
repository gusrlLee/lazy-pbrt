#ifndef GI_CORE_SAMPLE_HPP
#define GI_CORE_SAMPLE_HPP

#include "core/rng.hpp"
#include "core/geometry.hpp"

namespace gi {
    class Sample {
        public:
            static Vec2f InPixel() { return RNG::Get2D(0, 1); }
            static Vec3f Direction() { return RNG::Get3D(); }

    };

} // namespace gi
#endif