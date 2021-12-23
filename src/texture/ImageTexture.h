//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_IMAGETEXTURE_H
#define PROJECT_IMAGETEXTURE_H

class ImageTexture : public Texture {
public:
    const static int bytes_per_pixel = 3;
    constexpr static double color_scale = 1.0 / 255.0;

    ImageTexture() = default;

    explicit ImageTexture(const char *filename, int clamp_type = 1) {
        // load image using stb image
        auto components_per_pixel = bytes_per_pixel;
        data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

        // check integrity
        if (!data) {
            std::cerr << "ERROR: Could not load texture image file '" << filename << "'.\n";
            width = height = 0;
        }

        // set parameter
        bytes_per_scanline = bytes_per_pixel * width;
        uv_clamp_type = clamp_type;
    }

    ~ImageTexture() { delete[] data; }

    Color uv_color(double u, double v, const Vector3d &p) const override {
        // return cyan if no texture image found
        if (data == nullptr) return {0, 1, 1};

        // parse input
        if (uv_clamp_type == 0) {
            u = clamp(u, 0.0, 1.0);
            v = 1.0 - clamp(v, 0.0, 1.0);
        } else if (uv_clamp_type == 1) {
            u = u - floor(u);
            v = v - floor(v);
        }
        if (u < 0 || u > 1.0) std::cerr << "error u " << u << std::endl;
        if (v < 0 || v > 1.0) std::cerr << "error v " << v << std::endl;
        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);
        i = i >= width ? width - 1 : i;
        j = j >= height ? height - 1 : j;

        // locate pixel and return

        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;
        return {color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]};
    }

private:
    unsigned char *data{nullptr};
    int width{0}, height{0};
    int bytes_per_scanline{0};
    int uv_clamp_type{-1};  // 0: clamp 1: mod
};

#endif //PROJECT_IMAGETEXTURE_H
