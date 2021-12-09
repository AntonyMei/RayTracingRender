//
// Created by meiyixuan on 2021-12-09.
//
#include <iostream>
#include <vector>
#include "src/Vector3d.h"
#include "src/Pixel.h"

int main() {

    // Image
    const int image_width = 256;
    const int image_height = 256;
    auto image = std::vector<std::vector<Pixel>>();
    for (int idx = 0; idx < image_height; ++idx) {
        auto row = std::vector<Pixel>(image_width);
        image.push_back(std::move(row));
    }

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto r = double(i) / (image_width-1);
            auto g = double(j) / (image_height-1);
            auto b = 0.25;
            image[j][i].set(Color(r, g, b));
            image[j][i].write();
        }
    }
    std::cerr << "\nDone.\n";
}