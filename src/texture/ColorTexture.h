//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_COLORTEXTURE_H
#define PROJECT_COLORTEXTURE_H

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

#endif //PROJECT_COLORTEXTURE_H
