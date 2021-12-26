//
// Created by meiyixuan on 2021-12-25.
//

#ifndef PROJECT_BUMPTEXTURE_H
#define PROJECT_BUMPTEXTURE_H

class BumpTexture : public Texture {
public:
    const static int bytes_per_pixel = 3;
    constexpr static double color_scale = 1.0 / 255.0;

    BumpTexture() = default;

    explicit BumpTexture(const char *filename, int _bump_map_type, double _bump_scale = 0.01,
                         int clamp_type = 1) {
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
        bump_scale = _bump_scale;
        bump_map_type = _bump_map_type;
    }

    ~BumpTexture() { delete[] data; }

    Color uv_color(double u, double v, const Vector3d &p) const override { return {}; }

    Vector3d get_normal(double u, double v) {
        // return 0 if bump texture not found
        if (data == nullptr) return {0, 0, 1};

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

        // return based on type
        if (bump_map_type == 0) {
            // height map
            // calculate adjacent coordinates
            int ip1 = i + 1 == width ? 0 : i + 1;
            int im1 = i - 1 == -1 ? width - 1 : i - 1;
            int jp1 = j + 1 == height ? 0 : j + 1;
            int jm1 = j - 1 == -1 ? height - 1 : j - 1;

            // calculate normal
            Vector3d normal = Vector3d(-bump_scale * (h(ip1, j) - h(im1, j)),
                                       -bump_scale * (h(i, jp1) - h(i, jm1)),
                                       1);
            return normal;
        } else if (bump_map_type == 1) {
            // normal map
            auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;
            return {color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]};
        } else if (bump_map_type == 2) {
            // auto type detection
            auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;
            if (pixel[0] == pixel[1] && pixel[0] == pixel[2]) {
                // height map
                // calculate adjacent coordinates
                int ip1 = i + 1 == width ? 0 : i + 1;
                int im1 = i - 1 == -1 ? width - 1 : i - 1;
                int jp1 = j + 1 == height ? 0 : j + 1;
                int jm1 = j - 1 == -1 ? height - 1 : j - 1;

                // calculate normal
                Vector3d normal = Vector3d(-bump_scale * (h(ip1, j) - h(im1, j)),
                                           -bump_scale * (h(i, jp1) - h(i, jm1)),
                                           1);
                return normal;
            } else {
                // normal map
                return {color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]};
            }
        } else {
            std::cerr << "Unknown bump map type" << std::endl;
            return {0, 0, 1};
        }
    }

private:
    unsigned char *data{nullptr};
    int width{0}, height{0};
    int bytes_per_scanline{0};
    int uv_clamp_type{-1};  // 0: clamp 1: mod
    double bump_scale{0.01};
    int bump_map_type{-1};  // 0: height, 1: normal, 2: auto

    inline int h(int i, int j) {
        return (data + j * bytes_per_scanline + i * bytes_per_pixel)[0];
    }
};

#endif //PROJECT_BUMPTEXTURE_H
