#include "camera.h"

void Camera::render(const Primitive &world)
{
    // Initialization 
    this->init();

    // Render
    std::cout << "P3\n"
              << img_w << ' ' << img_h << "\n255\n";

    for (i32 j = 0; j < img_h; j++)
    {
        std::clog << "\rScanlines remaining : " << f64(j) / f64(img_h) * 100.0 << " % " << std::flush;
        for (i32 i = 0; i < img_w; i++)
        {
            Color px_color(0, 0, 0);
            for (i32 sample = 0; sample < SPP; sample++)
            {
                Ray r = get_ray(i, j);
                px_color += ray_color(r, max_depth, world);
            }
            write_color(std::cout, px_color * px_sample_scale);
        }
    }

    std::clog << "\rDone.                                                 \n";
}

void Camera::init()
{
    // Camera center point
    this->cam_center = Point3(0, 0, 0);
    this->px_sample_scale = 1.0 / SPP;

    // Calculate the image height, and ensure that it's at least.
    img_h = i32(img_w / aspect_ratio);
    img_h = (img_h < 1) ? 1 : img_h;

    // Camera settnig
    auto focal_length = 1.0;
    auto vport_h = 2.0;
    auto vport_w = vport_h * (f64(img_w) / img_h);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto vport_u = Vec3(vport_w, 0, 0);
    auto vport_v = Vec3(0, -vport_h, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    this->px_du = vport_u / f64(img_w);
    this->px_dv = vport_v / f64(img_h);
    
    // Calculate the location of the upper left pixel
    auto vport_upper_left = cam_center - vport_u / 2 - vport_v / 2 - Vec3(0, 0, focal_length);
    this->px_00 = vport_upper_left + 0.5 * (px_du + px_dv);
}

Vec3 Camera::sample_square() const 
{
    return Vec3(random_value() - 0.5, random_value() - 0.5, 0);
}

Ray Camera::get_ray(i32 i, i32 j)
{
    auto offset = sample_square();
    auto px_sample = px_00 + ((i + offset.x()) * px_du) + ((j + offset.y()) * px_dv);

    auto ray_o = cam_center;
    auto ray_d = px_sample - ray_o;

    return Ray(ray_o, ray_d);
}

Color Camera::ray_color(const Ray& r, i32 depth, const Primitive &world) const
{
    if (depth <= 0)
        return Color(0, 0, 0);

    hit_record rec;
    if (world.hit(r, Interval(0.001, inf), rec))
    {
        Ray scattered;
        Color attenuation;

        if (rec.mat->scatter(r, rec, attenuation, scattered))
        {
            return attenuation * ray_color(scattered, depth - 1, world);
        }
        return Color(0, 0, 0);
    }

    Vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}
