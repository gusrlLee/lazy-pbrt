#pragma once 

#include "math/vec3.h"

#include "integrator/integrator.h"
#include "core/color.h"
#include "core/hitrecord.h"
#include "core/material.h"

class PathTracer : public Integrator
{
public:
    // output image data
    Color* pOutput;

    std::shared_ptr<Camera> cam;
    std::shared_ptr<Scene> scn;
    
    void Render() override;
    Color Li(const Ray &r, I32 depth) const;

};