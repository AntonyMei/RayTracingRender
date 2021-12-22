#include <utility>

//
// Created by meiyixuan on 2021-12-22.
//

#ifndef PROJECT_PBRMATERIAL_H
#define PROJECT_PBRMATERIAL_H

class PBRMaterial : Material {
public:
    PBRMaterial(std::string name,
                Color _kd, const std::string &diffuse_map_name,
                Color _ks, double _shininess, const std::string &specular_map_name,
                Color _ke, const std::string &emission_map_name,
                double _ir, Color _tf)
            : material_name(std::move(name)),
              kd(_kd),
              diffuse_texture(diffuse_map_name.empty() ? nullptr :
                              std::make_shared<ImageTexture>(diffuse_map_name.c_str())),
              ks(_ks), shininess(_shininess),
              specular_texture(specular_map_name.empty() ? nullptr :
                               std::make_shared<ImageTexture>(specular_map_name.c_str())),
              ke(_ke), emission_texture(emission_map_name.empty() ? nullptr :
                                        std::make_shared<ImageTexture>(emission_map_name.c_str())),
              ir(_ir), transmission_filter(_tf) {
        auto kd_len = kd.squared_length();
        auto ks_len = ks.squared_length();
        auto re_len = transmission_filter.squared_length();
        auto total_energy = kd_len + ks_len + re_len + EPSILON;
        prob_diffuse = kd_len / total_energy;
        prob_specular = ks_len / total_energy;
        prob_refract = re_len / total_energy;
    }

    Color emit(double u, double v, const Point &p) const override {
        return ke * (emission_texture ? emission_texture->uv_color(u, v, p) : Color(1, 1, 1));
    }

    bool scatter(const Ray &ray_in, const Hit &hit, Ray &scattered_ray) const override {
        auto mode = scatter_type();
        if (mode == 0) {
            // get scatter direction
            auto scatter_direction = hit.normal + random_unit_vector();
            if (scatter_direction.near_zero())
                scatter_direction = hit.normal;

            // generate scattered rays
            scattered_ray = Ray(hit.hit_point, scatter_direction, ray_in.time());
            return true;
        } else if (mode == 1) {
            // get reflected direction (include perturbation for imperfect reflection)
            Vector3d reflected_dir = reflect(normalize(ray_in.direction()), hit.normal);
            reflected_dir += random_in_unit_sphere();

            // generate rays
            scattered_ray = Ray(hit.hit_point, reflected_dir, ray_in.time());
            return (dot(reflected_dir, hit.normal) > 0);
        } else if (mode == 2) {
            // calculate parameters
            double refraction_ratio = hit.front_face ? (1.0 / ir) : ir;

            Vector3d unit_direction = normalize(ray_in.direction());
            double cos_theta = fmin(dot(-unit_direction, hit.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            // generate new ray's direction
            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            Vector3d direction;
            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
                direction = reflect(unit_direction, hit.normal);
            else
                direction = refract(unit_direction, hit.normal, refraction_ratio);

            scattered_ray = Ray(hit.hit_point, direction, ray_in.time());
            return true;
        } else {
            std::cerr << "Unknown scatter type!" << std::endl;
            return false;
        }
    }

    inline Color brdf(const Ray &ray_in, const Ray &ray_out, const Hit &hit) const override {
        // Phong
        auto diffuse_color = kd * (diffuse_texture ? diffuse_texture->uv_color(hit.u, hit.v, hit.hit_point)
                                                   : Color(1, 1, 1));
        auto specular_color = ks *
    }

private:
    // basic property
    std::string material_name;

    // diffuse
    Color kd;
    std::shared_ptr<Texture> diffuse_texture;
    double prob_diffuse;

    // specular
    Color ks;
    double shininess; // specular exponent
    std::shared_ptr<Texture> specular_texture;
    double prob_specular;

    // refraction
    double ir;  // dielectric's eta of refraction
    Color transmission_filter;  // 1: no passing through
    double prob_refract;

    // emission
    Color ke;
    std::shared_ptr<Texture> emission_texture;

    int scatter_type() const {
        // 0 diffuse, 1 reflect, 2 refract
        auto val = random_double();
        if (val <= prob_diffuse) return 0;
        else if (val <= prob_diffuse + prob_specular) return 1;
        else return 2;
    }

    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }

};

#endif //PROJECT_PBRMATERIAL_H
