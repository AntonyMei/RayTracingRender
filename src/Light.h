//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_LIGHT_H
#define PROJECT_LIGHT_H

class Light : public Material {
};

class DiffuseLight : public Light {
public:
    explicit DiffuseLight(std::shared_ptr<Texture> texture, double _intensity = 1)
            : emit_texture(std::move(texture)), intensity(_intensity) {}

    explicit DiffuseLight(Color emit_color, double _intensity = 1) :
            emit_texture(std::make_shared<ColorTexture>(emit_color)),
            intensity(_intensity) {}

    bool scatter(const Ray &ray_in, const Hit &hit, Color &attenuation,
                 std::vector<Ray> &scattered_rays) const override {
        // lights don't scatter
        return false;
    }

    Color emit(double u, double v, const Point &p) const override {
        return emit_texture->uv_color(u, v, p) * intensity;
    }

private:
    std::shared_ptr<Texture> emit_texture;
    double intensity{1};
};

#endif //PROJECT_LIGHT_H