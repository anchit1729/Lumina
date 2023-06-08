//
// Created by Anchit Mishra on 2023-05-29.
//

#ifndef LUMINA_COLOR_H
#define LUMINA_COLOR_H

#include <lumina.h>
#include <vec3.h>

void write_color(std::ostream &os, color3 pixel_color)    {
    // Write [0, 255] value of each color component
    os << static_cast<int>(255.999 * pixel_color.x) << " "
       << static_cast<int>(255.999 * pixel_color.y) << " "
       << static_cast<int>(255.999 * pixel_color.z) << "\n";
}

#endif //LUMINA_COLOR_H
