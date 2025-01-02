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
    // Calculate the image height, and ensure that it's at least.
    img_h = i32(img_w / aspect_ratio);
    img_h = (img_h < 1) ? 1 : img_h;

    // Camera center point
    this->cam_center = lookfrom;
    this->px_sample_scale = 1.0 / SPP;

    // Camera settnig
    auto theta = degrees_to_radians(vfov);
    auto h = std::tan(theta / 2);

    auto vport_h = 2 * h * focus_dist;
    auto vport_w = vport_h * (f64(img_w) / img_h);

    // Calculate the u, v, w unit basis vectors for the caemra coordinate frame.
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto vport_u = vport_w * u;
    auto vport_v = vport_h * -v;

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    this->px_du = vport_u / f64(img_w);
    this->px_dv = vport_v / f64(img_h);
    
    // Calculate the location of the upper left pixel
    auto vport_upper_left = cam_center - (focus_dist * w) - vport_u / 2 - vport_v / 2;
    this->px_00 = vport_upper_left + 0.5 * (px_du + px_dv);

    auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;
}

Vec3 Camera::sample_square() const 
{
    return Vec3(random_value() - 0.5, random_value() - 0.5, 0);
}

Point3 Camera::defocus_disk_sample() const 
{
    auto p = random_in_unit_disk();
    return cam_center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}

Ray Camera::get_ray(i32 i, i32 j)
{
    // Construct a camera ray originating from the defocus disk and directed at a randomly 
    // sampled point around the pixel location i, j.
    auto offset = sample_square();
    auto px_sample = px_00 + ((i + offset.x()) * px_du) + ((j + offset.y()) * px_dv);

    auto ray_o = (defocus_angle <= 0) ? cam_center : defocus_disk_sample();
    auto ray_d = px_sample - ray_o;
    auto ray_tm = random_value();

    return Ray(ray_o, ray_d, ray_tm);
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
