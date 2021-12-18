#include <utility>

//
// Created by meiyixuan on 2021-12-19.
//

#ifndef PROJECT_ROTATEY_H
#define PROJECT_ROTATEY_H

class RotateY : public Hittable {
public:
    RotateY(std::shared_ptr<Hittable> p, double angle) : obj_ptr(std::move(p)) {
        auto radians = deg2rad(angle);
        sin_theta = sin(radians);
        cos_theta = cos(radians);
        has_box = obj_ptr->bounding_box(0, 1, transformed_aabb);

        Point min(inf, inf, inf);
        Point max(-inf, -inf, -inf);

        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 2; j++) {
                for (int k = 0; k < 2; k++) {
                    auto x = i * transformed_aabb.max().x() + (1 - i) * transformed_aabb.min().x();
                    auto y = j * transformed_aabb.max().y() + (1 - j) * transformed_aabb.min().y();
                    auto z = k * transformed_aabb.max().z() + (1 - k) * transformed_aabb.min().z();
                    auto new_x = cos_theta * x + sin_theta * z;
                    auto new_z = -sin_theta * x + cos_theta * z;
                    Vector3d tester(new_x, y, new_z);
                    for (int c = 0; c < 3; c++) {
                        min[c] = fmin(min[c], tester[c]);
                        max[c] = fmax(max[c], tester[c]);
                    }
                }
            }
        }
        transformed_aabb = AABB(min, max);
    }

    bool hit(const Ray &ray, double t_min, double t_max, Hit &hit) const override {
        // generate transformed ray
        auto origin = ray.origin();
        auto direction = ray.direction();
        origin[0] = cos_theta * ray.origin()[0] - sin_theta * ray.origin()[2];
        origin[2] = sin_theta * ray.origin()[0] + cos_theta * ray.origin()[2];
        direction[0] = cos_theta * ray.direction()[0] - sin_theta * ray.direction()[2];
        direction[2] = sin_theta * ray.direction()[0] + cos_theta * ray.direction()[2];
        Ray transformed_ray(origin, direction, ray.time());

        // miss
        if (!obj_ptr->hit(transformed_ray, t_min, t_max, hit)) return false;

        // hit
        auto p = hit.hit_point;
        auto normal = hit.normal;
        p[0] = cos_theta * hit.hit_point[0] + sin_theta * hit.hit_point[2];
        p[2] = -sin_theta * hit.hit_point[0] + cos_theta * hit.hit_point[2];
        normal[0] = cos_theta * hit.normal[0] + sin_theta * hit.normal[2];
        normal[2] = -sin_theta * hit.normal[0] + cos_theta * hit.normal[2];
        hit.hit_point = p;
        hit.set_face_normal(transformed_ray, normal);

        return true;
    }

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        output_box = transformed_aabb;
        return has_box;
    }

private:
    std::shared_ptr<Hittable> obj_ptr;
    double sin_theta{0}, cos_theta{1};
    bool has_box;
    AABB transformed_aabb;
};

#endif //PROJECT_ROTATEY_H
