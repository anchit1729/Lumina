//
// Created by Anchit Mishra on 2023-05-12.
//

#ifndef LUMINA_LUMINA_H
#define LUMINA_LUMINA_H

#include <cmath>
#include <cstdlib>
#include <limits>
#include <memory>
#include <iostream>
#include <fstream>

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees)    {
    // 180 degrees is PI radians
    return degrees*pi/180.0;
}

inline double random_double()   {
    // return a random real number in the range [0, 1]
    return rand()/(RAND_MAX+1.0);
}

inline double random_double(double min, double max) {
    // return a random real number in the range [min, max]
    return min+(max-min)*random_double();
}

inline int random_int(int min, int max) {
    // return a random integer in [min, max]
    return int(random_double(min, max + 1));
}

inline double clamp(double x, double min, double max)   {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

#include <ray.h>
#include <interval.h>
#include <vec3.h>

#endif //LUMINA_LUMINA_H
