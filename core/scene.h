#pragma once 

#include "core/types.h"
#include "core/ray.h"
#include "core/hittablelist.h"

class Scene 
{
public:
    std::shared_ptr<HitTableList> world;
    
};