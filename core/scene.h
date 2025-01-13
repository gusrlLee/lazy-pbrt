#pragma once 

#include "core/types.h"
#include "core/ray.h"
#include "core/hittable.h"
#include "core/hittablelist.h"
#include "core/color.h"

#include "shapes/quad.h"

class Scene 
{
public:
    Scene() = default;
    ~Scene() = default;
    // Sptr<HitTableList> world;
    // Sptr<HitTableList> lights;

    HitTableList world;
    HitTableList lights;

    Color background;
};