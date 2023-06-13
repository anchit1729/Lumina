//
// Created by Anchit Mishra on 2023-05-29.
//

#include <lumina.h>

#include <camera.h>
#include <color.h>
#include <hittable_list.h>
#include <sphere.h>
#include <material.h>

hittable_list random_scene() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color3(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color3::random() * color3::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color3::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color3(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color3(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}


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
    std::cerr << "Setting up image attributes...\n";
    // Image dimensions
    const auto aspect_ratio = 3.0/2.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;
    const int max_depth = 50;

    std::cerr << "Building world scene...\n";
    // World Definition
    auto world = random_scene();

//    auto material_ground = make_shared<lambertian>(color3(0.8, 0.8, 0.0));
//    auto material_center = make_shared<lambertian>(color3(0.1, 0.2, 0.5));
//    auto material_left   = make_shared<dielectric>(1.5);
//    auto material_right  = make_shared<metal>(color3(0.8, 0.6, 0.2), 0.0);

//    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
//    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
//    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
//    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0), -0.45, material_left));
//    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

//    auto material_ground = make_shared<lambertian>(color3(0.8, 0.8, 0));
//    auto material_center = make_shared<lambertian>(color3(0.7, 0.3, 0.3)); // diffuse surface sphere in center
//    //auto material_left = make_shared<metal>(color3(0.8, 0.8, 0.8), 0.3); // metallic sphere on the left
    //auto material_center = make_shared<dielectric>(1.5); // dielectric sphere in the center
//    auto material_left = make_shared<dielectric>(1.5);
//    auto material_right = make_shared<metal>(color3(0.8, 0.6, 0.2), 1.0);


//    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
//    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
//    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
//    // negative radius trick for hollow spheres/bubbles
//    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.4, material_left));
//    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    std::cerr << "Creating camera...\n";
    // Camera
    // Standard 90 deg FOV
    //camera camera(point3(-2,2,1), point3(0,0,-1), vec3(0,1,0), 90, aspect_ratio);
    // Narrow 20 deg FOV
    point3 lookfrom = {13, 2, 3};
    point3 lookat = {0, 0, 0};
    vec3 upwards = {0, 1, 0};
    double distance_to_focus = 10.0;
    auto aperture = 0.1;
    camera camera(lookfrom, lookat, upwards, 20, aspect_ratio, aperture, distance_to_focus);

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
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(image_file, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone.\n";
}