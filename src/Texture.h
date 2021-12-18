#include <utility>

//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_TEXTURE_H
#define PROJECT_TEXTURE_H

class Texture {
public:
    virtual Color color(double u, double v, const Point &p) const = 0;
};

class ColorTexture : public Texture {
public:
    ColorTexture() = default;

    explicit ColorTexture(Color c) : texture_color(c) {}

    ColorTexture(double r, double g, double b) : texture_color{r, g, b} {}

    Color color(double u, double v, const Point &p) const override {
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

    Color color(double u, double v, const Point &p) const override {
        auto sines = sin(step_size * p.x()) * sin(step_size * p.y()) * sin(step_size * p.z());
        if (sines < 0)
            return odd->color(u, v, p);
        else
            return even->color(u, v, p);
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

    Color color(double u, double v, const Point &p) const override {
        return Color(1, 1, 1) * noise.turbulence(scale * p);
    }

private:
    // scale = 1: very low freq
    // scale = 4: moderate frequency
    PerlinNoise noise;
    double scale{1};
};

#endif //PROJECT_TEXTURE_H
