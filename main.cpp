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

gi::HittableList CornellBox()
{
    HittableList world;

    auto red = MakeShared<Lambertian>(Color(.65, .05, .05));
    auto white = MakeShared<Lambertian>(Color(.73, .73, .73));
    auto green = MakeShared<Lambertian>(Color(.12, .45, .15));
    auto light = MakeShared<DiffuseLight>(Color(15, 15, 15));
    auto aluminum = MakeShared<Metal>(Color(0.8, 0.85, 0.88), 0.0);
    auto glass = MakeShared<Dielectric>(1.5);

    world.Add(MakeShared<Quad>(Point3(555, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), green));
    world.Add(MakeShared<Quad>(Point3(0, 0, 0), Vec3(0, 555, 0), Vec3(0, 0, 555), red));
    world.Add(MakeShared<Quad>(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), light));
    world.Add(MakeShared<Quad>(Point3(0, 0, 0), Vec3(555, 0, 0), Vec3(0, 0, 555), white));
    world.Add(MakeShared<Quad>(Point3(555, 555, 555), Vec3(-555, 0, 0), Vec3(0, 0, -555), white));
    world.Add(MakeShared<Quad>(Point3(0, 0, 555), Vec3(555, 0, 0), Vec3(0, 555, 0), white));

    SPtr<Hittable> box1 = Box(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = MakeShared<RotateY>(box1, 15);
    box1 = MakeShared<Translate>(box1, Vec3(265, 0, 295));
    world.Add(box1);

    // Sptr<HitTable> box2 = Box(Point3(0, 0, 0), Point3(165, 165, 165), white);
    // box2 = MakeShared<RotateY>(box2, -18);
    // box2 = MakeShared<Translate>(box2, Vec3(130, 0, 65));
    // world.Add(box2);

    world.Add(MakeShared<Sphere>(Point3(190, 90, 190), 90, glass));

    return world;
}

int main()
{
    // setting camera
    SPtr<Camera> pCam = MakeShared<Camera>();
    pCam->aspectRatio = 1.0f;
    pCam->imgWidth = 600;
    pCam->spp = 500;
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
    HittableList world = CornellBox();
    world = HittableList(MakeShared<BVHNode>(world));

    auto emptyMat = SPtr<Material>();
    HittableList lights;
    lights.Add(MakeShared<Quad>(Point3(343, 554, 332), Vec3(-130, 0, 0), Vec3(0, 0, -105), emptyMat));
    lights.Add(MakeShared<Sphere>(Point3(190, 90, 190), 90, emptyMat));

    // setting integrator
    SPtr<Path> pt = MakeShared<Path>();
    pt->cam = pCam;
    pt->scn = scn;

    pt->Render(world, lights);

    return 0;
}