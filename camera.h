//
// Created by Anchit Mishra on 2023-06-07.
//

#ifndef LUMINA_CAMERA_H
#define LUMINA_CAMERA_H

#include <lumina.h>

class camera    {
public:
    camera(point3 lookfrom, point3 lookat, vec3 upwards, double v_fov, double aspect_ratio, double aperture, double focus_distance)    {
        auto theta = degrees_to_radians(v_fov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        w = unit(lookfrom - lookat);
        u = unit(cross(upwards, w));
        v = cross(w, u);

        auto focal_length = 1.0;

        origin = lookfrom;
        horizontal = focus_distance * viewport_width * u;
        vertical = focus_distance * viewport_height * v;
        lower_left_corner = origin - horizontal/2 - vertical/2 - focus_distance * w;

        lens_radius = aperture / 2;
    }

    ray get_ray(double u, double v) const   {
        vec3 d = lower_left_corner + u*horizontal + v*vertical - origin;
        point3 o = origin;
        return ray(o, d);
    }

private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
};

#endif //LUMINA_CAMERA_H
