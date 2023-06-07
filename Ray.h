//
// Created by Anchit Mishra on 2023-05-29.
//

#ifndef LUMINA_RAY_H
#define LUMINA_RAY_H

#include <Lumina.h>
#include <Vector3.h>

class Ray   {
public:
    Vector3 origin;
    Vector3 direction;
    Ray() {}
    Ray(Point3 &origin, Vector3 &direction): origin(origin), direction(direction)   {}
    Point3 at(const double t)   { return origin + direction * t; }
};

#endif //LUMINA_RAY_H
