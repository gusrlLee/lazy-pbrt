#include <iostream>

#include "core/material.hpp"
#include "core/hittablelist.hpp"
#include "core/scene.hpp"
#include "core/camera.hpp"
#include "accel/bvh.hpp"

#include "shapes/quad.hpp"
#include "shapes/sphere.hpp"

#include "integrator/path.hpp"

using namespace gi;

int main()
{
    // setting camera
    SPtr<Camera> pCam = MakeShared<Camera>();
    pCam->aspectRatio = 1.0f;
    pCam->imgWidth = 600;
    pCam->spp = 10;
    pCam->maxDepth = 50;

    pCam->vFov = 40;
    pCam->lookfrom = Point3(278, 278, -800);
    pCam->lookat = Point3(278, 278, 0);
    pCam->vup = Vec3(0, 1, 0);

    pCam->defocusAngle = 0.0f;
    pCam->Init();

    // setting scene
    SPtr<Scene> scn = MakeShared<Scene>();
    scn->background = Color(0.0f);
    scn->SetCornellBox();

    auto emptyMat = SPtr<Material>();
    HittableList lights;
    lights.Add(MakeShared<Quad>(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), emptyMat));
    lights.Add(MakeShared<Sphere>(Point3(190, 90, 190), 90, emptyMat));

    // setting integrator
    SPtr<Path> pt = MakeShared<Path>();
    pt->cam = pCam;
    pt->scn = scn;

    pt->Render(scn->world, lights);

    return 0;
}
