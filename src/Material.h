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
    Metal(const Color &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

    Color get_albedo() { return albedo; }

    bool scatter(const Ray &r_in, const Hit &hit, Color &attenuation,
                 std::vector<Ray> &scattered_rays) const override {
        // get reflected direction (include perturbation for imperfect reflection)
        Vector3d reflected_dir = reflect(normalize(r_in.direction()), hit.normal);
        reflected_dir += fuzz * random_in_unit_sphere();

        // generate rays
        scattered_rays.emplace_back(hit.hit_point, reflected_dir);
        attenuation = albedo;
        return (dot(reflected_dir, hit.normal) > 0);
    }

private:
    Color albedo;
    double fuzz;
};

class Dielectric : public Material {
public:
    explicit Dielectric(double index_of_refraction) : ir(index_of_refraction) {}

    bool scatter(const Ray &ray_in, const Hit &hit, Color &attenuation,
                 std::vector<Ray> &scattered_rays) const override {
        attenuation = Color(1.0, 1.0, 1.0);
        double refraction_ratio = hit.front_face ? (1.0 / ir) : ir;

        Vector3d unit_direction = normalize(ray_in.direction());
        Vector3d refracted = refract(unit_direction, hit.normal, refraction_ratio);

        scattered_rays.emplace_back(hit.hit_point, refracted);
        return true;
    }

private:
    // eta of this material (index of refraction)
    double ir;
};

#endif //PROJECT_MATERIAL_H
