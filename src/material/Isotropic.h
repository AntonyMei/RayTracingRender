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

    bool scatter(const Ray &ray_in, const Hit &hit, Ray &scattered_ray) const override {
        scattered_ray = Ray(hit.hit_point, random_in_unit_sphere(), ray_in.time());
        return true;
    }

    Color brdf(const Ray &ray_in, const Ray &ray_out, const Hit &hit) const override {
        return albedo->uv_color(hit.u, hit.v, hit.hit_point);
    }

private:
    std::shared_ptr<Texture> albedo;
};

#endif //PROJECT_ISOTROPIC_H
