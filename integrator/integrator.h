#pragma once 
#include <iostream>

#include "core/types.h"

#include "core/ray.h"
#include "core/hitrecord.h"
#include "core/scene.h"
#include "core/camera.h"


class Integrator 
{
public:
    virtual void Render(const HitTable &world, const HitTable &lights) {}
    
    const I32 kTileSizeX = 8;
    const I32 kTileSizeY = 8;
};