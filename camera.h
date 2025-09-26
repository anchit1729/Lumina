//
// Created by Anchit Mishra on 2023-06-07.
//

#ifndef LUMINA_CAMERA_H
#define LUMINA_CAMERA_H

#include <lumina.h>

class camera    {
public:
    camera(point3 lookfrom, point3 lookat, vec3 upwards, double v_fov, double aspect_ratio, double aperture, double focus_distance, interval shutter_time)    {
        auto theta = degrees_to_radians(v_fov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        open_time = shutter_time.min;
        close_time = shutter_time.max;

        w = unit(lookfrom - lookat);
        u = unit(cross(upwards, w));
        v = cross(w, u);

        auto focal_length = 1.0;

        origin = lookfrom;
        horizontal = focus_distance * viewport_width * u;
        vertical = focus_distance * viewport_height * v;
        upper_left_corner = origin - horizontal/2 - vertical/2 - focus_distance * w;

        lens_radius = aperture / 2;
    }

    ray get_ray(double u, double v) const   {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = this->u * rd.x + this->v * rd.y;
        vec3 d = upper_left_corner + u*horizontal + v*vertical - origin - offset;
        point3 o = origin + offset;
        return ray(o, d, random_double(open_time, close_time));
    }

private:
    int image_width;
    int image_height;
    double distance_to_focus;
    double aperture;
    point3 origin;
    point3 upper_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
    double open_time;
    double close_time;
};

#endif //LUMINA_CAMERA_H
