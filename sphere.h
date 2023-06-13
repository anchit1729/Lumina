//
// Created by Anchit Mishra on 2023-06-07.
//

#ifndef LUMINA_sphere_H
#define LUMINA_sphere_H

#include <hittable.h>
#include <vec3.h>

using namespace std;

class material;

class sphere : public hittable  {
public:
    // Data items
    point3 centre;
    double radius;
    shared_ptr<material> material_ptr;
    // Function definitions
    sphere() {}
    sphere(point3 centre, double radius, shared_ptr<material> m) : centre(centre), radius(radius), material_ptr(m)   {}
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& hit_rec) const override;
};

bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &hit_rec) const {
    vec3 ray_to_circle_centre = r.origin - centre;
    auto a = dot(r.direction, r.direction);
    auto half_b = dot(ray_to_circle_centre, r.direction);
    auto c = ray_to_circle_centre.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;
    if (discriminant < 0) return false; // No intersection
    auto root = (-half_b - sqrt(discriminant)) / a;
    if (root < t_min || t_max < root)   {
        root = (-half_b + sqrt(discriminant) / a);
        if (root < t_min || t_max < root) return false; // No intersection
    }
    hit_rec.root = root;
    hit_rec.point = r.at(root);
    vec3 outward_normal = (hit_rec.point - centre) / radius;
    hit_rec.set_face_normal(r, outward_normal);
    hit_rec.material_ptr = material_ptr;
    return true;
}

#endif //LUMINA_sphere_H
