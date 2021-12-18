//
// Created by meiyixuan on 2021-12-09.
// This file contains the definition of Pixel.
//

#ifndef PROJECT_PIXEL_H
#define PROJECT_PIXEL_H

class Pixel {
public:
    Pixel() : pixel_color(), write_flag(false), sample_count(1) {}

    void set(const Color &color, int samples_per_pixel) {
        pixel_color = color;
        sample_count = samples_per_pixel;
    }

    Color get_color() const { return pixel_color; }

    int get_sample_count() const { return sample_count; }

    void write(std::ofstream &out) {
        // a pixel can only be written once for correctness
        if (!write_flag) {
            write_flag = true;
        } else {
            throw std::runtime_error("A pixel is written twice");
        }

        // divide by sample count and gamma correct
        auto r = pixel_color.x();
        auto g = pixel_color.y();
        auto b = pixel_color.z();
        auto scale = 1.0 / sample_count;
        r = sqrt(scale * r);
        g = sqrt(scale * g);
        b = sqrt(scale * b);

        // scale to [0, 255] and output
        out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
            << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
            << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
    }

private:
    bool write_flag;
    Color pixel_color;
    int sample_count;
};

#endif //PROJECT_PIXEL_H
