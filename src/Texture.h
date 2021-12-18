//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_TEXTURE_H
#define PROJECT_TEXTURE_H

class Texture {
public:
    virtual Color uv_color(double u, double v, const Point &p) const = 0;
};


// procedural textures
class ColorTexture : public Texture {
public:
    ColorTexture() = default;

    explicit ColorTexture(Color c) : texture_color(c) {}

    ColorTexture(double r, double g, double b) : texture_color{r, g, b} {}

    Color uv_color(double u, double v, const Point &p) const override {
        return texture_color;
    }

private:
    Color texture_color;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture() = default;

    CheckerTexture(std::shared_ptr<Texture> _odd, std::shared_ptr<Texture> _even, double _step_size = 10)
            : odd(std::move(_odd)), even(std::move(_even)), step_size(_step_size) {}

    CheckerTexture(const Color &_odd, const Color &_even, double _step_size = 10)
            : odd(std::make_shared<ColorTexture>(_odd)),
              even(std::make_shared<ColorTexture>(_even)),
              step_size(_step_size) {}

    Color uv_color(double u, double v, const Point &p) const override {
        auto sines = sin(step_size * p.x()) * sin(step_size * p.y()) * sin(step_size * p.z());
        if (sines < 0)
            return odd->uv_color(u, v, p);
        else
            return even->uv_color(u, v, p);
    }

private:
    double step_size{10};
    std::shared_ptr<Texture> odd;
    std::shared_ptr<Texture> even;
};

class PerlinTexture : public Texture {
public:
    PerlinTexture() = default;

    explicit PerlinTexture(double _scale) : scale(_scale) {}

    Color uv_color(double u, double v, const Point &p) const override {
        // if we use "Color(1,1,1) * 0.5 * (1.0 + noise.noise(scale * p))",
        // we will get a lighter looking Perlin texture.
        return Color(1, 1, 1) * noise.turbulence(scale * p);
    }

private:
    // scale = 1: very low freq
    // scale = 4: moderate frequency
    PerlinNoise noise;
    double scale{1};
};

class MarbleTexture : public Texture {
public:
    MarbleTexture() = default;

    explicit MarbleTexture(double _scale) : scale(_scale) {}

    Color uv_color(double u, double v, const Point &p) const override {
        // This will render a marble texture.
        return Color(1, 1, 1) * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turbulence(p)));
    }

private:
    PerlinNoise noise;
    double scale{4};
};

// image texture
class ImageTexture : public Texture {
public:
    const static int bytes_per_pixel = 3;
    constexpr static double color_scale = 1.0 / 255.0;

    ImageTexture() = default;

    explicit ImageTexture(const char *filename) {
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
    }

    ~ImageTexture() { delete[] data; }

    Color uv_color(double u, double v, const Vector3d &p) const override {
        // return cyan if no texture image found
        if (data == nullptr) return {0, 1, 1};

        // parse input
        u = clamp(u, 0.0, 1.0);
        v = 1.0 - clamp(v, 0.0, 1.0);
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

};

#endif //PROJECT_TEXTURE_H
