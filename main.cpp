//
// Created by Anchit Mishra on 2023-05-29.
//

#include <lumina.h>

#include <camera.h>
#include <color.h>
#include <hittable_list.h>
#include <sphere.h>
#include <material.h>

double compute_hit_sphere(const point3& centre, const double radius, const ray& r) {
    // The surface of the sphere is characterized by the vector equation
    // (P - C) . (P - C) = r^2
    // (O + td - C) . (O + td - C) = r^2
    // t^2(d.d) - 2t(C - O).d + (O - C).(O - C) = r^2
    auto ray_origin_to_centre = r.origin - centre;
    auto a = dot(r.direction, r.direction);
    auto half_b = (dot(ray_origin_to_centre, r.direction));
    auto c = dot(ray_origin_to_centre, ray_origin_to_centre) - radius*radius;
    auto discriminant = (half_b*half_b - a*c);
    if (discriminant < 0)   return -1.0;
    double result = (-1.0*half_b - sqrt(discriminant)) / (a);
    return result;
}

color3 ray_color(const ray& r, const hittable& world, int recursion_depth)  {
    // Check recursion depth to prevent stack fill-up
    if (recursion_depth <= 0)    {
        return color3(0, 0, 0);
    }
    hit_record hit_rec;
    // Use t_min = 0.001 to avoid shadow acne issues
    if (world.hit(r, 0.001, infinity, hit_rec)) {
        ray scattered;
        color3 attenuation;
        if (hit_rec.material_ptr->scatter(r, hit_rec, attenuation, scattered))  {
            return attenuation * ray_color(scattered, world, recursion_depth - 1);
        }
        return color3(0, 0, 0);
    }
    vec3 unit_direction = unit(r.direction);
    auto t = 0.5*(unit_direction.y+1.0);
    return (1.0-t)*color3(1.0, 1.0, 1.0) + t*color3(0.5, 0.7, 1.0);
}

int main() {
    // Image dimensions
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 1000;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 1000;
    const int max_depth = 50;

    // World Definition
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color3(0.8, 0.8, 0));
    auto material_center = make_shared<lambertian>(color3(0.7, 0.3, 0.3));
    auto material_left = make_shared<metal>(color3(0.8, 0.8, 0.8));
    auto material_right = make_shared<metal>(color3(0.8, 0.6, 0.2));

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Camera
    camera camera;

    // Draw the image
    // Set up file to write to
    std::ofstream image_file("img.ppm");
    // PPM format header
    image_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color3 pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                // multiple samples for anti-aliasing
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = camera.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(image_file, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
}