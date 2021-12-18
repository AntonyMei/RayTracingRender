//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_MARBLETEXTURE_H
#define PROJECT_MARBLETEXTURE_H

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

#endif //PROJECT_MARBLETEXTURE_H
