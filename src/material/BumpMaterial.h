//
// Created by meiyixuan on 2021-12-25.
//

#ifndef PROJECT_BUMPMATERIAL_H
#define PROJECT_BUMPMATERIAL_H

class BumpMaterial : public Material {
public:
    BumpMaterial() = default;

    BumpMaterial(const std::string &mat_pth, const std::string &bump_map_name)
            : bump_map(bump_map_name.empty() ? nullptr :
                       std::make_shared<BumpTexture>((mat_pth + bump_map_name).c_str())) {}

    bool scatter(const Ray &ray_in, Hit &hit, Ray &scattered_ray) const override { return false; }

    Color brdf(const Ray &ray_in, const Ray &ray_out, const Hit &hit) const override { return {}; }

    Vector3d get_normal(double u, double v) {
        return bump_map->get_normal(u, v);
    }

private:
    std::shared_ptr<BumpTexture> bump_map;
};

#endif //PROJECT_BUMPMATERIAL_H
