//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_PERLINTEXTURE_H
#define PROJECT_PERLINTEXTURE_H

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

#endif //PROJECT_PERLINTEXTURE_H
