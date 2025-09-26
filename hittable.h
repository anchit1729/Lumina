//
// Created by Anchit Mishra on 2023-06-07.
//

#ifndef LUMINA_HITTABLE_H
#define LUMINA_HITTABLE_H

#include <lumina.h>
#include <aabb.h>

class material;

struct hit_record   {
    point3 point;
    vec3 normal;
    shared_ptr<material> material_ptr;
    // the parameter value of the root
    double root;
    // texture coordinates
    double u;
    double v;
    // to track whether the intersection face is pointing towards the camera (visible) or not (invisible)
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal)    {
        front_face = dot(r.direction, outward_normal) < 0;
        normal = front_face ? (outward_normal / outward_normal.length()) : (-outward_normal / outward_normal.length());
    }
};

class hittable  {
public:
    virtual bool hit(const ray& r, interval t_interval, hit_record& rec) const = 0;
    virtual aabb bounding_box() const = 0;
};

#endif //LUMINA_HITTABLE_H
