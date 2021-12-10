//
// Created by meiyixuan on 2021-12-09.
// This file contains the definition of Pixel.
//

#ifndef PROJECT_PIXEL_H
#define PROJECT_PIXEL_H

#include "Vector3d.h"

class Pixel {
public:
    Pixel() : pixel_color(), write_flag(false) {}

    void set(const Color &color) { pixel_color = color; }

    Color get() { return pixel_color; }

    void write() {
        // a pixel can only be written once for correctness
        if (!write_flag) {
            write_flag = true;
        } else {
            throw std::runtime_error("A pixel is written twice");
        }

        // scale to [0, 255]
        std::cout << static_cast<int>(255.999 * pixel_color.x()) << ' '
                  << static_cast<int>(255.999 * pixel_color.y()) << ' '
                  << static_cast<int>(255.999 * pixel_color.z()) << '\n';
    }

private:
    bool write_flag;
    Color pixel_color;
};

#endif //PROJECT_PIXEL_H
