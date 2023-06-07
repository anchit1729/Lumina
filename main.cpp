//
// Created by Anchit Mishra on 2023-05-29.
//

#include <Lumina.h>
#include <Ray.h>
#include <Color.h>

bool detect_hit_sphere(const Point3& centre, const double radius, const Ray& r) {
    // The surface of the sphere is characterized by the equation
    // (x - c.x)^2 + (y - c.y)^2 + (z - z.y)^2 = r^2
    // Which in vector form is
    // (P - C) . (P - C) = r^2
    // (O + td - C) . (O + td - C) = r^2
    // O.O + t(O.d) - O.C + t(O.d) + t^2(d.d) - t(C.d) - C.O - t(C.d) + C.C = r^2
    // t^2(d.d) - 2t(C.d - O.d) - 2(O.C) + O.O + C.C = r^2
    auto a = dot(r.direction, r.direction);
    auto b = 2 * (dot(r.origin, r.direction) - dot(centre, r.direction));
    auto c = dot(r.origin, r.origin) + dot(centre, centre) - 2*dot(r.origin, centre) - radius*radius;
    auto discriminant = (b*b - 4*a*c);
    return discriminant > 0;
}

Color3 ray_color(const Ray& r)  {
    Vector3 unit_direction = unit(r.direction);
    if (detect_hit_sphere(Point3(0, 0, -1), 0.5, r))
        return Color3(1, 0, 0);
    auto t = 0.5*(unit_direction.y+1.0);
    return (1-t)*Color3(1, 1, 1) + t*Color3(0.5, 0.7, 1);
}

int main() {
    // Image dimensions
    const auto aspect_ratio = 16.0/9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * aspect_ratio;
    auto focal_length = 1.0;

    auto origin = Point3(0, 0, 0);
    auto horizontal = Vector3(viewport_width, 0, 0);
    auto vertical = Vector3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - Vector3(0, 0, focal_length);

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
            Vector3 direction = lower_left_corner + u*horizontal + v*vertical - origin;
            Ray r(origin, direction);
            Color3 pixel_color = ray_color(r);
            write_color(image_file, pixel_color);
        }
    }
    std::cerr << "\nDone.\n";
}