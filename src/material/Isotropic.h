#include <utility>

//
// Created by meiyixuan on 2021-12-19.
//

#ifndef PROJECT_ISOTROPIC_H
#define PROJECT_ISOTROPIC_H

class Isotropic : public Material {
public:
    explicit Isotropic(Color color) : albedo(std::make_shared<ColorTexture>(color)) {}

    explicit Isotropic(std::shared_ptr<Texture> texture) : albedo(std::move(texture)) {}

    bool scatter(const Ray &ray_in, const Hit &hit, Color &attenuation,
                 std::vector<Ray> &scattered_rays) const override {
        scattered_rays.emplace_back(hit.hit_point, random_in_unit_sphere(), ray_in.time());
        attenuation = albedo->uv_color(hit.u, hit.v, hit.hit_point);
        return true;
    }

private:
    std::shared_ptr<Texture> albedo;
};

#endif //PROJECT_ISOTROPIC_H
