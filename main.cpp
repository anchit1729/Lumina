//
// Created by Anchit Mishra on 2023-05-29.
//

#include <lumina.h>

#include <bvh.h>
#include <camera.h>
#include <color.h>
#include <hittable_list.h>
#include <sphere.h>
#include <moving_sphere.h>
#include <material.h>
#include <texture.h>

hittable_list cover_scene_book_one() {
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

hittable_list bouncing_balls_with_texture()    {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color3(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    // checkboard texture
    auto checker = make_shared<checker_texture>(0.32, color3(0.2, 0.3, 0.1), color3(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 centre(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((centre - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color3::random() * color3::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto centre_2 = centre + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<moving_sphere>(centre, centre_2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color3::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(centre, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(centre, 0.2, sphere_material));
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

    world = hittable_list(make_shared<bvh_node>(world));

    return world;
}

hittable_list checkered_spheres() {
    hittable_list world;

    auto checker = make_shared<checker_texture>(0.32, color3(0.2, 0.3, 0.1), color3(0.9, 0.9, 0.9));

    world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
    world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

    return world;
}

hittable_list textured_globe() {
    auto earth_texture = make_shared<image_texture>("earth.jpg");
    auto earth_surface = make_shared<lambertian>(earth_texture);
    auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);
    return hittable_list(globe);
}

hittable_list perlin_spheres() {
    hittable_list world;

    auto pertext = make_shared<noise_texture>(4);
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(pertext)));
    world.add(make_shared<sphere>(point3(0, 2, 0), 2, make_shared<lambertian>(pertext)));

    return world;
}

color3 ray_color(const ray& r, const hittable& world, int recursion_depth)  {
    // Check recursion depth to prevent stack fill-up
    if (recursion_depth <= 0)    {
        return color3(0, 0, 0);
    }
    hit_record hit_rec;
    // Use t_min = 0.001 to avoid shadow acne issues
    interval hit_interval = interval(0.001, infinity);
    if (world.hit(r, hit_interval, hit_rec)) {
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
    std::clog << "Setting up image attributes...\n";
    // Image dimensions
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 900;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
    const int max_depth = 50;

    std::clog << "Building world scene...\n";
    // World Definition
    auto world = perlin_spheres();

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

    std::clog << "Creating camera...\n";
    // Camera
    // Standard 90 deg FOV
    //camera camera(point3(-2,2,1), point3(0,0,-1), vec3(0,1,0), 90, aspect_ratio);
    // Narrow 20 deg FOV
    // camera for everything up till textured globe
    point3 lookfrom = {13, 2, 3};
    point3 lookat = {0, 0, 0};
    vec3 upwards = {0, 1, 0};
    double distance_to_focus = 10.0;
    auto aperture = 0.1;
    interval shutter_capture_interval = interval(0.0, 1.0);

    // camera for textured globe
    lookfrom = {0, 0, 12};
    lookat = {0, 0, 0};

    // camera for perlin scene
    lookfrom = {13, 2, 3};
    lookat = {0, 0, 0};

    camera camera(lookfrom, lookat, upwards, 20, aspect_ratio, aperture, distance_to_focus, shutter_capture_interval);

    std::clog << "Creating output file...\n";
    // Draw the image
    // Set up file to write to
    std::ofstream image_file("motion_blur.ppm");
    // PPM format header
    image_file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    std::clog << "Rendering image...\n";
    for (int j = image_height-1; j >= 0; --j) {
        std::clog << "\rScanlines remaining: " << j << ' ' << std::flush;
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
    std::clog << "\nDone.\n";
}