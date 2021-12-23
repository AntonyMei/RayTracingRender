#include <utility>

//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_METAL_H
#define PROJECT_METAL_H

class Metal : public Material {
public:
    Metal(const Color &a, double f) : albedo(std::make_shared<ColorTexture>(a)), fuzz(f < 1 ? f : 1) {}

    Metal(std::shared_ptr<Texture> a, double f) : albedo(std::move(a)), fuzz(f < 1 ? f : 1) {}

    bool scatter(const Ray &ray_in, Hit &hit, Ray &scattered_ray) const override {
        // get reflected direction (include perturbation for imperfect reflection)
        Vector3d reflected_dir = reflect(normalize(ray_in.direction()), hit.normal);
        reflected_dir += fuzz * random_in_unit_sphere();

        // generate rays
        scattered_ray = Ray(hit.hit_point, reflected_dir, ray_in.time());
        return (dot(reflected_dir, hit.normal) > 0);
    }

    Color brdf(const Ray &ray_in, const Ray &ray_out, const Hit &hit) const override {
        return albedo->uv_color(hit.u, hit.v, hit.hit_point);
    }

private:
    std::shared_ptr<Texture> albedo;
    double fuzz;
};

#endif //PROJECT_METAL_H
