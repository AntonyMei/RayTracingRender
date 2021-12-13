//
// Created by meiyixuan on 2021-12-11.
//

#ifndef PROJECT_MATERIAL_H
#define PROJECT_MATERIAL_H

class Material {
public:
    virtual bool scatter(const Ray &ray_in, const Hit &hit, Color &attenuation,
                         std::vector<Ray> &scattered_rays) const = 0;
};

class Lambertian : public Material {
public:
    explicit Lambertian(const Color &a) : albedo(a) {}

    Color get_albedo() { return albedo; }

    bool scatter(const Ray &ray_in, const Hit &hit, Color &attenuation,
                 std::vector<Ray> &scattered_rays) const override {
        // get scatter direction
        auto scatter_direction = hit.normal + random_unit_vector();
        if (scatter_direction.near_zero())
            scatter_direction = hit.normal;

        // generate scattered rays
        scattered_rays.emplace_back(hit.hit_point, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    Color albedo;
};

class Metal : public Material {
public:
    explicit Metal(const Color &a) : albedo(a) {}

    Color get_albedo() { return albedo; }

    bool scatter(const Ray &r_in, const Hit &hit, Color &attenuation,
                 std::vector<Ray> &scattered_rays) const override {
        Vector3d reflected_dir = reflect(normalize(r_in.direction()), hit.normal);
        scattered_rays.emplace_back(hit.hit_point, reflected_dir);
        attenuation = albedo;
        return (dot(reflected_dir, hit.normal) > 0);
    }

private:
    Color albedo;
};

#endif //PROJECT_MATERIAL_H
