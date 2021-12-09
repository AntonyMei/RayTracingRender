//
// Created by meiyixuan on 2021-12-09.
//

#ifndef PROJECT_PIXEL_H
#define PROJECT_PIXEL_H

#include "Vector3d.h"

class Pixel {
public:
    Pixel() : pixel_color() {}

    void set(const Color &color) { pixel_color = color; }

    Color get() { return pixel_color; }

    void write() {
        // scale to [0, 255]
        std::cout << static_cast<int>(255.999 * pixel_color.x()) << ' '
                  << static_cast<int>(255.999 * pixel_color.y()) << ' '
                  << static_cast<int>(255.999 * pixel_color.z()) << '\n';
    }

private:
    Color pixel_color;
};

#endif //PROJECT_PIXEL_H
