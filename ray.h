//
// Created by Anchit Mishra on 2023-05-29.
//

#ifndef LUMINA_RAY_H
#define LUMINA_RAY_H

#include <lumina.h>
#include <vec3.h>

class ray   {
public:
    vec3 origin;
    vec3 direction;
    ray() {}
    ray(point3 &origin, vec3 &direction): origin(origin), direction(direction)   {}
    point3 at(const double t) const  { return origin + direction * t; }
};

#endif //LUMINA_RAY_H
