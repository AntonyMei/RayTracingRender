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
              kd(_kd), diffuse_texture(std::make_shared<ImageTexture>(diffuse_map_name.c_str())),
              ks(_ks), shininess(_shininess),
              specular_texture(std::make_shared<ImageTexture>(specular_map_name.c_str())),
              ke(_ke), emission_texture(std::make_shared<ImageTexture>(emission_map_name.c_str())),
              ir(_ir), transmission_filter(_tf) {
        auto kd_len = kd.squared_length();
        auto ks_len = ks.squared_length();
        auto re_len = transmission_filter.squared_length();
        auto total_energy = kd_len + ks_len + re_len + EPSILON;
        prob_diffuse = kd_len / total_energy;
        prob_specular = ks_len / total_energy;
        prob_refract = re_len / total_energy;
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

};

#endif //PROJECT_PBRMATERIAL_H
