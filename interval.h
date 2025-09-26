//
// Created by Anchit Mishra on 2025-06-22.
//

#ifndef LUMINA_INTERVAL_H
#define LUMINA_INTERVAL_H

#include <lumina.h>

class interval {
public:
    double min;
    double max;

    interval() {
        // create an open limitless interval
        min = +INFINITY;
        max = -INFINITY;
    }

    interval(const double& i_min, const double& i_max) : min(i_min), max(i_max) {}

    interval(const interval& a, const interval& b) {
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }

    bool is_empty()    {
        return min > max;
    }

    interval compute_overlap(interval second_interval)   {
        if (second_interval.min > max || min > second_interval.max) return interval(INFINITY, -INFINITY);
        else return interval(std::max(min, second_interval.min), std::min(max, second_interval.max));
    }

    double size() const   {
        return max - min;
    }

    bool contains(double x) {
        return min <= x && x <= max;
    }

    bool surrounds(double x) {
        return min < x && x < max;
    }

    double clamp(double val) {
        if (val > max) return max;
        if (val < min) return min;
        return val;
    }

    interval extend(double d) {
        double padding = d / 2;
        return interval(min - padding, max + padding);
    }

    static const interval empty, universe;
};

const interval interval::empty = interval(INFINITY, -INFINITY);
const interval interval::universe = interval(-INFINITY, INFINITY);

inline double random_double(interval range) {
    range.min + (range.max - range.min) * random_double();
}


#endif //LUMINA_INTERVAL_H
