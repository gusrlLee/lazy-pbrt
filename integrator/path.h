#pragma once 

#include "math/vec3.h"

#include "integrator/integrator.h"

class PathTracer : public Integrator
{
public:
    // output image data
    int* pxData;

    std::shared_ptr<Camera> cam;
    std::shared_ptr<Scene> scn;
    
    void Render() override;

};