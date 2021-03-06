//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_DIELECTRIC_H
#define PROJECT_DIELECTRIC_H

class Dielectric : public Material {
public:
    explicit Dielectric(double index_of_refraction, Color tf = Vector3d(1, 1, 1))
            : ir(index_of_refraction), transmission_filter(tf) {}

    bool scatter(const Ray &ray_in, Hit &hit, Ray &scattered_ray) const override {
        // calculate parameters
        double refraction_ratio = hit.front_face ? (1.0 / ir) : ir;

        Vector3d unit_direction = normalize(ray_in.direction());
        double cos_theta = fmin(dot(-unit_direction, hit.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

        // generate new ray's direction
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vector3d direction;
        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
            direction = reflect(unit_direction, hit.normal);
            hit.scatter_mode = 1;
        } else {
            direction = refract(unit_direction, hit.normal, refraction_ratio);
            hit.scatter_mode = 2;
        }


        scattered_ray = Ray(hit.hit_point, direction, ray_in.time());
        return true;
    }

    Color brdf(const Ray &ray_in, const Ray &ray_out, const Hit &hit) const override {
        return transmission_filter;
    }

private:
    // eta of this material (index of refraction)
    double ir;
    Color transmission_filter{1.0, 1.0, 1.0};

    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};

#endif //PROJECT_DIELECTRIC_H
