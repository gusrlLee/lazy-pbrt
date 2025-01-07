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
            Point3 center(a + 0.9 *  Random::Value(), 0.2, b + 0.9 *  Random::Value());

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
    std::shared_ptr<Camera> pCam = std::make_shared<Camera>();
    pCam->aspectRatio = 16.0 / 9.0;
    pCam->imgWidth = 800;
    pCam->spp = 100;
    pCam->maxDepth = 50;

    pCam->lookfrom = Point3(13, 2, 3);
    pCam->lookat = Point3(0, 0, 0);
    pCam->vup = Vec3(0, 1, 0);
    pCam->defocusAngle = 0.6;
    pCam->focusDist = 10.0f;

    pCam->Init();

    std::shared_ptr<Scene> pScene = std::make_shared<Scene>();
    pScene->world = std::make_shared<HitTableList>(Test2());

    std::shared_ptr<PathTracer> pIntegrator = std::make_shared<PathTracer>();
    pIntegrator->cam = pCam;
    pIntegrator->scn = pScene;

    pIntegrator->Render();
    return 0;
}