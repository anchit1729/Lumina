//
// Created by Anchit Mishra on 2023-06-07.
//

#ifndef LUMINA_CAMERA_H
#define LUMINA_CAMERA_H

#include <lumina.h>

class camera    {
public:
    camera()    {
        auto aspect_ratio = 16.0/9.0;
        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        origin = point3(0, 0, 0);
        horizontal = vec3(viewport_width, 0.0, 0.0);
        vertical = vec3(0.0, viewport_height, 0.0);
        lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
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
};

#endif //LUMINA_CAMERA_H
