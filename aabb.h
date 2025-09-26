//
// Created by Anchit Mishra on 2023-10-22.
//

#ifndef LUMINA_AABB_H
#define LUMINA_AABB_H

#include <lumina.h>

// Axis-Aligned Bounding Box Class
class aabb  {
public:
    interval x, y, z;

    aabb() {}

    aabb(const interval& x, const interval& y, const interval& z) : x(x), y(y), z(z) {
        pad_to_minimums();
    }

    aabb(const point3& p1, const point3& p2) {
        // construct a bounding box using extreme points
        x = (p1.x < p2.x) ? interval(p1.x, p2.x) : interval(p2.x, p1.x);
        y = (p1.y < p2.y) ? interval(p1.y, p2.y) : interval(p2.y, p1.y);
        z = (p1.z < p2.z) ? interval(p1.z, p2.z) : interval(p2.z, p1.z);

        pad_to_minimums();
    }

    aabb(const aabb& b1, const aabb& b2)    {
        // construct a bounding box using two bounding boxes
        x = interval(b1.x, b2.x);
        y = interval(b1.y, b2.y);
        z = interval(b1.z, b2.z);

        pad_to_minimums();
    }

    interval axis_interval(int n) const {
        switch (n) {
            case 0: return x;
            case 1: return y;
            case 2: return z;
        }
    }

    bool hit(const ray& r, interval ray_interval) const {
        const vec3 ray_origin = r.origin;
        const vec3 ray_direction = r.direction;

        for (int axis = 0; axis < 3; axis++) {
            interval axis_of_interest = axis_interval(axis);
            double inverse_direction = 1.0 / ray_direction[axis];
            double t0 = (axis_of_interest.min - ray_origin[axis]) * inverse_direction;
            double t1 = (axis_of_interest.max - ray_origin[axis]) * inverse_direction;

            if (t0 < t1) {
                if (t0 > ray_interval.min) ray_interval.min = t0;
                if (t1 < ray_interval.max) ray_interval.max = t1;
            } else {
                if (t1 > ray_interval.min) ray_interval.min = t1;
                if (t0 < ray_interval.max) ray_interval.max = t0;
            }

            if (ray_interval.min >= ray_interval.max) return false;
        }
        return true;
    }

    int longest_axis() const {
        if (x.size() > y.size()) {
            return x.size() > z.size() ? 0 : 2;
        } else {
            return y.size() > z.size() ? 1 : 2;
        }
    }

    static const aabb empty, universe;

private:
    point3 minimum;
    point3 maximum;

    void pad_to_minimums() {
        double delta = 0.0001;

        if (x.size() < delta) x = x.expand(delta);
        if (y.size() < delta) y = y.expand(delta);
        if (z.size() < delta) z = z.expand(delta);
    }
};

const aabb aabb::empty = aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);

#endif //LUMINA_AABB_H
