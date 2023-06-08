//
// Created by Anchit Mishra on 2023-05-29.
//

#include <lumina.h>

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
    // Image dimensions
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // World Definition
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * aspect_ratio;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    // Draw the image
    // Set up file to write to
    std::ofstream image_file("img.ppm");
    // PPM format header
    image_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width-1);
            auto v = double(j) / (image_height-1);
            vec3 direction = lower_left_corner + u*horizontal + v*vertical;
            ray r(origin, direction);
            color3 pixel_color = ray_color(r, world);
            write_color(image_file, pixel_color);
        }
    }
    std::cerr << "\nDone.\n";
}