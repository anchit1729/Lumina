//
// Created by Anchit Mishra on 2023-05-29.
//

#ifndef LUMINA_COLOR_H
#define LUMINA_COLOR_H

#include <lumina.h>
#include <vec3.h>

void write_color(std::ostream &os, color3 pixel_color, int samples_per_pixel)    {
    auto r = pixel_color.x;
    auto g = pixel_color.y;
    auto b = pixel_color.z;

    // Divide color values by the number of samples + color correct using gamma = 2.0
    auto scale = 1.0 / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    // Write [0, 255] value of each color component
    os << static_cast<int>(255.999 * clamp(r, 0.0, 0.999)) << " "
       << static_cast<int>(255.999 * clamp(g, 0.0, 0.999)) << " "
       << static_cast<int>(255.999 * clamp(b, 0.0, 0.999)) << "\n";
}

#endif //LUMINA_COLOR_H
