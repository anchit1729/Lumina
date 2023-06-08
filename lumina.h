//
// Created by Anchit Mishra on 2023-05-12.
//

#ifndef LUMINA_LUMINA_H
#define LUMINA_LUMINA_H

#include <cmath>
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
    return degrees * pi / 180.0;
}

#include <ray.h>
#include <vec3.h>

#endif //LUMINA_LUMINA_H
