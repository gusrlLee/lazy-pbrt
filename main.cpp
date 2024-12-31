#include "camera.h"
#include "sphere.h"
#include "vec3.h"
#include "world.h"
#include "material.h"

int main(int argc, const char **argv) {

  // Make world of spheres
  World world;
  auto material_ground = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
  auto material_center = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
  auto material_left   = std::make_shared<Dielectric>(1.50);
  auto material_bubble   = std::make_shared<Dielectric>(1.00 / 1.50);
  auto material_right  = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

  world.add(std::make_shared<Sphere>(Point3( 0.0, -100.5, -1.0), 100.0, material_ground));
  world.add(std::make_shared<Sphere>(Point3( 0.0,    0.0, -1.2),   0.5, material_center));
  world.add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.5, material_left));
  world.add(std::make_shared<Sphere>(Point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
  world.add(std::make_shared<Sphere>(Point3( 1.0,    0.0, -1.0),   0.5, material_right));

  // Camera setting
  Camera cam;
  cam.aspect_ratio = 16.0 / 9.0;
  cam.img_w = 720;
  cam.SPP = 100;
  cam.max_depth = 50;

  // Start rendering
  cam.render(world);

  return 0;
}
