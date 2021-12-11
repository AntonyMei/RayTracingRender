//
// Created by meiyixuan on 2021-12-09.
// This file contains the definition of Pixel.
//

#ifndef PROJECT_PIXEL_H
#define PROJECT_PIXEL_H

#include "Vector3d.h"
#include "Utils.h"

class Pixel {
public:
    Pixel() : pixel_color(), write_flag(false), sample_count(1) {}

    void set(const Color &color, int samples_per_pixel) {
        pixel_color = color;
        sample_count = samples_per_pixel;
    }

    Color get_color() const { return pixel_color; }

    int get_sample_count() const { return sample_count; }

    void write() {
        // a pixel can only be written once for correctness
        if (!write_flag) {
            write_flag = true;
        } else {
            throw std::runtime_error("A pixel is written twice");
        }

        // scale to [0, 255]
        pixel_color *= 1.0 / sample_count;
        std::cout << static_cast<int>(256 * clamp(pixel_color.x(), 0.0, 0.999)) << ' '
                  << static_cast<int>(256 * clamp(pixel_color.y(), 0.0, 0.999)) << ' '
                  << static_cast<int>(256 * clamp(pixel_color.z(), 0.0, 0.999)) << '\n';
    }

private:
    bool write_flag;
    Color pixel_color;
    int sample_count;
};

#endif //PROJECT_PIXEL_H
