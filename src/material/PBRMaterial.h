#include <utility>

//
// Created by meiyixuan on 2021-12-22.
//

#ifndef PROJECT_PBRMATERIAL_H
#define PROJECT_PBRMATERIAL_H

class PBRMaterial : public Material {
public:
    PBRMaterial(std::string name, const std::string &mat_pth,
                Color _kd, const std::string &diffuse_map_name,
                Color _ks, double _shininess, const std::string &specular_map_name,
                Color _ke, const std::string &emission_map_name,
                double _ir, Color _tf, double _dissolve,
                double _light_sample_probability = 0, Vector3d _sun_dir = Vector3d())
            : material_name(std::move(name)),
              kd(_kd),
              diffuse_texture(diffuse_map_name.empty() ? nullptr :
                              std::make_shared<ImageTexture>((mat_pth + diffuse_map_name).c_str())),
              ks(_ks), shininess(_shininess),
              specular_texture(specular_map_name.empty() ? nullptr :
                               std::make_shared<ImageTexture>((mat_pth + specular_map_name).c_str())),
              ke(_ke), emission_texture(emission_map_name.empty() ? nullptr :
                                        std::make_shared<ImageTexture>((mat_pth + emission_map_name).c_str())),
              ir(_ir), transmission_filter(_tf), dissolve(_dissolve),
              sample_light_prob(_light_sample_probability), sun_dir(_sun_dir) {
        auto kd_len = kd.squared_length();
        auto ks_len = ks.squared_length();
        auto total_energy = kd_len + ks_len + EPSILON;
        prob_refract = 1 - dissolve;
        prob_diffuse = dissolve * kd_len / total_energy;
        prob_specular = dissolve * ks_len / total_energy;
    }

    Color emit(double u, double v, const Point &p) const override {
        return ke * (emission_texture ? emission_texture->uv_color(u, v, p) : Color(1, 1, 1));
    }

    bool scatter(const Ray &ray_in, Hit &hit, Ray &scattered_ray) const override {
        auto mode = scatter_type();
        if (mode == 0) {
            // mode = 0: defuse material
            if (hit.remaining_bounce == 1) {
                // sample to light direction at last bounce
                // get scatter direction
                auto scatter_direction = sun_dir;
                if (dot(scatter_direction, hit.normal) <= 0)
                    scatter_direction = hit.normal + random_unit_vector();
                if (scatter_direction.near_zero())
                    scatter_direction = hit.normal;
                // generate scattered rays
                scattered_ray = Ray(hit.hit_point, scatter_direction, ray_in.time());
                hit.scatter_mode = 0;
                return true;
            }
            if (random_double() > sample_light_prob) {
                // a random direction sampler
                // get scatter direction
                auto scatter_direction = hit.normal + random_unit_vector();
                if (scatter_direction.near_zero())
                    scatter_direction = hit.normal;
                // generate scattered rays
                scattered_ray = Ray(hit.hit_point, scatter_direction, ray_in.time());
                hit.scatter_mode = 0;
                return true;
            } else {
                // sample more light for better convergence
                // get scatter direction
                auto scatter_direction = sun_dir + 0.02 * random_in_unit_sphere();
                if (dot(scatter_direction, hit.normal) <= 0)
                    scatter_direction = hit.normal + random_unit_vector();
                if (scatter_direction.near_zero())
                    scatter_direction = hit.normal;
                // generate scattered rays
                scattered_ray = Ray(hit.hit_point, scatter_direction, ray_in.time());
                hit.scatter_mode = 0;
                return true;
            }
        } else if (mode == 1) {
            // get reflected direction (include perturbation for imperfect reflection)
            Vector3d reflected_dir = reflect(normalize(ray_in.direction()), hit.normal);
            // reflected_dir += random_in_unit_sphere();

            // generate rays
            scattered_ray = Ray(hit.hit_point, reflected_dir, ray_in.time());
            hit.scatter_mode = 1;
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
            if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
                direction = reflect(unit_direction, hit.normal);
                hit.scatter_mode = 1;
            } else {
                direction = refract(unit_direction, hit.normal, refraction_ratio);
                hit.scatter_mode = 2;
            }
            scattered_ray = Ray(hit.hit_point, direction, ray_in.time());
            return true;
        } else {
            std::cerr << "Unknown scatter type!" << std::endl;
            return false;
        }
    }

    inline Color brdf(const Ray &ray_in, const Ray &ray_out, const Hit &hit) const override {
        // Phong
        if (hit.scatter_mode == 0 || hit.scatter_mode == 1) {
            auto diffuse_color = diffuse_texture ? kd * diffuse_texture->uv_color(hit.u, hit.v, hit.hit_point)
                                                 : kd;
            auto specular_color_base = specular_texture ? ks * specular_texture->uv_color(hit.u, hit.v, hit.hit_point)
                                                        : ks;
            Vector3d reflected_dir = reflect(normalize(ray_in.direction()), hit.normal);
            auto specular_exponent = pow(dot(reflected_dir, normalize(ray_out.direction())), shininess)
                                     / dot(hit.normal, -normalize(ray_in.direction()));
            specular_exponent = fmax(EPSILON * EPSILON, specular_exponent);
            specular_exponent = fmin(5.0, specular_exponent);
//            if (random_double() < 0.0000001)
//                std::cerr << "specular_exponent " << specular_exponent << std::endl;

            return diffuse_color + specular_color_base * specular_exponent;
        } else if (hit.scatter_mode == 2) {
            return transmission_filter;
        } else {
            std::cerr << "Unknown scatter mode" << std::endl;
            return {0, 1, 1};
        }
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
    Color transmission_filter;  // attenuation to refracted ray
    double dissolve;
    double prob_refract;

    // emission
    Color ke;
    std::shared_ptr<Texture> emission_texture;

    // sampler
    double sample_light_prob{0};
    Vector3d sun_dir;

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
