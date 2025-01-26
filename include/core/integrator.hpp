#ifndef GI_CORE_INTEGRATOR_HPP
#define GI_CORE_INTEGRATOR_HPP

#include "core/hittable.hpp"
#include "core/material.hpp"

namespace gi {
    class Integrator {
        public:
            // Main rendering function
            virtual void Render(const Hittable &world, const Hittable &lights) {}
            
            const Int kTileSizeX = 8;
            const Int kTileSizeY = 8;
    };

}

#endif