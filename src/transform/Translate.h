#include <utility>

//
// Created by meiyixuan on 2021-12-19.
//

#ifndef PROJECT_TRANSLATE_H
#define PROJECT_TRANSLATE_H

class Translate : public Transform {
public:
    Translate(std::shared_ptr<Hittable> obj, const Vector3d &_displacement)
            : obj_ptr(std::move(obj)), displacement(_displacement) {}

    bool hit(const Ray &ray, double t_min, double t_max, Hit &hit) const override {
        // transform
        Ray transformed_ray(ray.origin() - displacement, ray.direction(), ray.time());

        // miss
        if (!obj_ptr->hit(transformed_ray, t_min, t_max, hit))
            return false;

        // hit
        hit.hit_point += displacement;
        hit.set_face_normal(transformed_ray, hit.normal);

        return true;
    }

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        // inf object
        if (!obj_ptr->bounding_box(time0, time1, output_box)) return false;

        // normal
        output_box = AABB(output_box.min() + displacement,
                          output_box.max() + displacement);
        return true;
    }

private:
    std::shared_ptr<Hittable> obj_ptr;
    Vector3d displacement;
};

#endif //PROJECT_TRANSLATE_H
