#include "core/types.h"
#include "math/vec3.h"
#include "core/color.h"

#include "core/random.h"

#include <iostream>

#include "core/camera.h"
#include "core/scene.h"
#include "core/hittablelist.h"
#include "core/material.h"

#include "material/lambertian.h"
#include "material/metal.h"
#include "material/dielectric.h"

#include "integrator/path.h"

#include "shapes/sphere.h"


HitTableList Test1()
{
    HitTableList world;
    auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<Dielectric>(1.50);
    auto material_bubble   = std::make_shared<Dielectric>(1.00 / 1.50);
    auto material_right  = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

    world.Add(std::make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.Add(std::make_shared<Sphere>(Point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.Add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.Add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    world.Add(std::make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));

    return world;
}


HitTableList Test2()
{
    HitTableList world;
    auto grountMat = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, grountMat));

    for (I32 a = -11; a < 11; a++)
    {
        for (I32 b = -11; b < 11; b++)
        {
            auto choose_mat = Random::Value();
            Point3 center(a + 0.9 * Random::Value(), 0.2, b + 0.9 * Random::Value());

            if ((center - Point3(4, 0.2, 0)).Length() > 0.9)
            {
                std::shared_ptr<Material> sphere_material;
                if (choose_mat < 0.8)
                {
                    auto albedo = Random::GenColor() * Random::GenColor();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    auto center2 = center + Vec3(0, Random::Value(0, 0.5), 0);
                    world.Add(std::make_shared<Sphere>(center, center2, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95)
                {
                    // metal
                    auto albedo = Random::GenColor(0.5, 1);
                    auto fuzz = Random::Value(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else
                {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.Add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.Add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.Add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

int main()
{
    // setting camera
    std::shared_ptr<Camera> pCam = std::make_shared<Camera>();

    pCam->aspectRatio = 16.0 / 9.0;
    pCam->imgWidth = 720;
    pCam->spp = 10;
    pCam->maxDepth = 10;

    pCam->vFov = 20;
    pCam->lookfrom = Point3(13,2,3);
    pCam->lookat = Point3(0,0,0);
    pCam->vup = Vec3(0, 1, 0);

    pCam->defocusAngle = 0.6f;
    pCam->focusDist = 10.0f;
    pCam->Init();

    // setting scene of rendering
    std::shared_ptr<Scene> pScene = std::make_shared<Scene>();
    pScene->world = std::make_shared<HitTableList>(Test2());

    // setting path tracer
    std::shared_ptr<PathTracer> pIntegrator = std::make_shared<PathTracer>();
    pIntegrator->cam = pCam;
    pIntegrator->scn = pScene;

    pIntegrator->Render();
    
    return 0;
}