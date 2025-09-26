//
// Created by Anchit Mishra on 2023-10-20.
//

#ifndef LUMINA_MOVING_SPHERE_H
#define LUMINA_MOVING_SPHERE_H

#include <hittable.h>

class moving_sphere: public hittable   {
public:
    moving_sphere() {}
    moving_sphere(point3 start_centre, point3 stop_centre, double start_time, double stop_time, double radius, shared_ptr<material> mat) :
        start_centre(start_centre), stop_centre(stop_centre), start_time(start_time), stop_time(stop_time), radius(radius), material_ptr(mat) {
        auto extrema = vec3(radius, radius, radius);
        aabb box1 = aabb(start_centre - extrema, start_centre + extrema);
        aabb box2 = aabb(stop_centre - extrema, stop_centre + extrema);
        bbox = aabb(box1, box2);
    };

    virtual bool hit(const ray& r, interval t_interval, hit_record& hit_rec) const override {
        vec3 ray_to_circle_centre = r.origin - centre(r.timestamp);
        auto a = dot(r.direction, r.direction);
        auto half_b = dot(ray_to_circle_centre, r.direction);
        auto c = ray_to_circle_centre.length_squared() - radius*radius;
        auto discriminant = half_b*half_b - a*c;
        if (discriminant < 0) return false; // No intersection
        auto root = (-half_b - sqrt(discriminant)) / a;
        if (root < t_interval.min || t_interval.max < root)   {
            root = (-half_b + sqrt(discriminant) / a);
            if (root < t_interval.min || t_interval.max < root) return false; // No intersection
        }
        hit_rec.root = root;
        hit_rec.point = r.at(root);
        vec3 outward_normal = (hit_rec.point - centre(r.timestamp)) / radius;
        hit_rec.set_face_normal(r, outward_normal);
        hit_rec.material_ptr = material_ptr;
        return true;
    }

    point3 centre(double time) const {
        // assuming linear motion for simplicity
        auto new_centre = start_centre + ((stop_centre - start_centre) / (stop_time - start_time)) * (time - start_time);
        return new_centre;
    }
    aabb bounding_box() const override { return bbox; }

private:
    point3 start_centre;
    point3 stop_centre;
    double start_time;
    double stop_time;
    double radius;
    aabb bbox;
    shared_ptr<material> material_ptr;
};

#endif //LUMINA_MOVING_SPHERE_H
