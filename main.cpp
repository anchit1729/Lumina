//
// Created by Anchit Mishra on 2023-05-29.
//

#include <lumina.h>

#include <camera.h>
#include <color.h>
#include <hittable_list.h>
#include <sphere.h>

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

color3 ray_color(const ray& r, const hittable& world)  {
    hit_record hit_rec;
    if (world.hit(r, 0, infinity, hit_rec)) {
        return 0.5*(hit_rec.normal+color3(1, 1, 1));
    }
    vec3 unit_direction = unit(r.direction);
    auto t = 0.5*(unit_direction.y+1.0);
    return (1.0-t)*color3(1.0, 1.0, 1.0) + t*color3(0.5, 0.7, 1.0);
}

int main() {
    std::cerr << "Setting up image attributes...\n";
    // Image dimensions
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;

    std::cerr << "Building world scene...\n";
    // World Definition
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    std::cerr << "Creating camera...\n";
    // Camera
    camera camera;

    std::cerr << "Creating output file...\n";
    // Draw the image
    // Set up file to write to
    std::ofstream image_file("img.ppm");
    // PPM format header
    image_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    std::cerr << "Rendering image...\n";
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color3 pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                // multiple samples for anti-aliasing
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                ray r = camera.get_ray(u, v);
                pixel_color += ray_color(r, world);
            }
            write_color(image_file, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
}