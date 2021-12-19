//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_LAMBERTIAN_H
#define PROJECT_LAMBERTIAN_H

class Lambertian : public Material {
public:
    explicit Lambertian(const Color &a) : albedo(std::make_shared<ColorTexture>(a)) {}

    explicit Lambertian(std::shared_ptr<Texture> a) : albedo(std::move(a)) {}

    bool scatter(const Ray &ray_in, const Hit &hit, Color &alb,
                 std::vector<Ray> &scattered_rays, double &pdf) const override {
        // get scatter direction
        OrthonormalBasis onb;
        onb.build_from_w(hit.normal);
        auto direction = onb.local(random_cosine_direction());

        // generate scattered rays
        scattered_rays.emplace_back(hit.hit_point, normalize(direction), ray_in.time());
        alb = albedo->uv_color(hit.u, hit.v, hit.hit_point);
        pdf = dot(onb.w(), normalize(direction)) / pi;
        return true;
    }

    double scattering_pdf(const Ray &ray_in, const Hit &hit, const Ray &scattered)
    const override {
        auto cosine = dot(hit.normal, normalize(scattered.direction()));
        return cosine < 0 ? 0 : cosine / pi;
    }

private:
    std::shared_ptr<Texture> albedo;
};

#endif //PROJECT_LAMBERTIAN_H
