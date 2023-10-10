//
// Created by Anchit Mishra on 2023-06-07.
//

#ifndef LUMINA_HITTABLE_H
#define LUMINA_HITTABLE_H

#include <lumina.h>

class material;

struct hit_record   {
    point3 point;
    vec3 normal;
    shared_ptr<material> material_ptr;
    double root;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal)    {
        front_face = dot(r.direction, outward_normal) < 0;
        normal = front_face ? (outward_normal / outward_normal.length()) : (-outward_normal / outward_normal.length());
    }
};

class hittable  {
public:
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
};

#endif //LUMINA_HITTABLE_H
