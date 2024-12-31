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

    // MAIN REDNERING FUNCTION 
    void render(const Primitive &world);

private:
    void init();

    Ray get_ray(i32 i, i32 j);
    Vec3 sample_square() const;

    // Ray traversal 
    Color ray_color(const Ray& r, i32 depth, const Primitive& world) const;

    // Camera settnig
    i32 img_h;
    Point3 cam_center = Point3(0, 0, 0);
    Point3 px_00;
    Point3 px_du;
    Point3 px_dv;
    
    f64 px_sample_scale;

};
