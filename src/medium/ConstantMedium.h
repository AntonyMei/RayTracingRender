#include <utility>

//
// Created by meiyixuan on 2021-12-19.
//

#ifndef PROJECT_CONSTANTMEDIUM_H
#define PROJECT_CONSTANTMEDIUM_H

class ConstantMedium : public ParticipatingMedium {
public:
    ConstantMedium(shared_ptr<Hittable> _boundary, double d, const shared_ptr<Texture> &texture)
            : boundary(std::move(_boundary)), neg_inv_density(-1 / d),
              phase_function(make_shared<Isotropic>(texture)) {}

    ConstantMedium(std::shared_ptr<Hittable> _boundary, double d, Color color)
            : boundary(std::move(_boundary)), neg_inv_density(-1 / d),
              phase_function(std::make_shared<Isotropic>(color)) {}

    bool hit(const Ray &ray, double t_min, double t_max, Hit &hit) const override {

        // check boundary conditions
        Hit rec1, rec2;
        if (!boundary->hit(ray, -inf, inf, rec1)) return false;
        if (!boundary->hit(ray, rec1.t + 0.0001, inf, rec2)) return false;
        if (rec1.t < t_min) rec1.t = t_min;
        if (rec2.t > t_max) rec2.t = t_max;
        if (rec1.t >= rec2.t) return false;
        if (rec1.t < 0) rec1.t = 0;

        // check if the ray goes through medium (i.e. no scattering)
        const auto ray_length = ray.direction().length();
        const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
        const auto hit_distance = neg_inv_density * log(random_double());
        if (hit_distance > distance_inside_boundary) return false;

        // set hit info
        hit.t = rec1.t + hit_distance / ray_length;
        hit.hit_point = ray.at(hit.t);
        hit.normal = Vector3d(1, 0, 0);  // arbitrary
        hit.front_face = true;     // also arbitrary
        hit.mat_ptr = phase_function;

        return true;
    }

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        return boundary->bounding_box(time0, time1, output_box);
    }

private:
    std::shared_ptr<Hittable> boundary;
    std::shared_ptr<Material> phase_function;
    double neg_inv_density;
};

#endif //PROJECT_CONSTANTMEDIUM_H
