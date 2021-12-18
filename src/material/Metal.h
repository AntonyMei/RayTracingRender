//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_METAL_H
#define PROJECT_METAL_H

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
        scattered_rays.emplace_back(hit.hit_point, reflected_dir, r_in.time());
        attenuation = albedo;
        return (dot(reflected_dir, hit.normal) > 0);
    }

private:
    Color albedo;
    double fuzz;
};

#endif //PROJECT_METAL_H
