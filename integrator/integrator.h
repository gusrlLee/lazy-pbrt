#pragma once 
#include <iostream>
#include "core/ray.h"
#include "core/hitrecord.h"
#include "core/scene.h"
#include "core/camera.h"

class Integrator 
{
public:
    virtual void Render() {}
};