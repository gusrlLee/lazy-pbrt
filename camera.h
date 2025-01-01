#pragma once

#include "types.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"

#include "primitive.h"
#include "material.h"

class Camera
{
public:
    f64 aspect_ratio = 16.0 / 9.0; // Ratio of image width over height
    i32 img_w = 720; // Rendered image width in pixel count 
    i32 SPP = 10; // Sample Per Pixel 
    i32 max_depth = 10; // Ray max depth
    
    f64 vfov = 90; // Vertical view angle (field of view) 
    Point3 lookfrom = Point3(0, 0, 0); // Point camera is looking from
    Point3 lookat = Point3(0, 0, -1); // Point camera is looking at 
    Vec3 vup = Vec3(0, 1, 0); // Camera-releative "up" direction 
    
    f64 defocus_angle = 0; // Variation angle of rays through each pixel 
    f64 focus_dist = 10; // Distance from camera lookfrom point to plane of perfect focus 

    // MAIN REDNERING FUNCTION 
    void render(const Primitive &world);

private:
    void init();

    Ray get_ray(i32 i, i32 j);
    Vec3 sample_square() const;
    Point3 defocus_disk_sample() const;

    // Ray traversal 
    Color ray_color(const Ray& r, i32 depth, const Primitive& world) const;

    // Camera settnig
    i32 img_h;
    Point3 cam_center = Point3(0, 0, 0);
    Point3 px_00;
    Vec3 px_du;
    Vec3 px_dv;
    Vec3 u, v, w; // Camera frame basis vectors
    Vec3 defocus_disk_u;  // Defocus disk horizontal radius 
    Vec3 defocus_disk_v;  // Defocus disk vertical radius 

    f64 px_sample_scale;
};
