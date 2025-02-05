#ifndef GI_CORE_INTEGRATOR_HPP
#define GI_CORE_INTEGRATOR_HPP

#include "core/geometry.hpp"
#include "core/materials.hpp"
#include "core/camera.hpp"
#include "core/scene.hpp"

namespace gi
{
    class Integrator
    {
    public:
        const Int kTileSizeX = 8;
        const Int kTileSizeY = 8;

        virtual ~Integrator() = default;
        virtual void Render() { printf("[ERROR] You have to develop your integrator to render image\n"); }

    private:
    };
} // namespace gi

#endif // GI_CORE_INTEGRATOR_HPP