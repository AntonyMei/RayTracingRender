//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_CHECKERTEXTURE_H
#define PROJECT_CHECKERTEXTURE_H

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

#endif //PROJECT_CHECKERTEXTURE_H
