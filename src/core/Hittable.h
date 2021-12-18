//
// Created by meiyixuan on 2021-12-10.
// This file contains all hittable objects.
//

#ifndef PROJECT_HITTABLE_H
#define PROJECT_HITTABLE_H

// base class for all hittable objects
class Hittable {
public:
    Hittable() : mat_ptr(nullptr) {}

    explicit Hittable(std::shared_ptr<Material> m) : mat_ptr(std::move(m)) {}

    virtual bool hit(const Ray &ray, double t_min, double t_max, Hit &hit) const = 0;

    virtual bool bounding_box(double time0, double time1, AABB &output_box) const = 0;

protected:
    shared_ptr<Material> mat_ptr;
};

#endif //PROJECT_HITTABLE_H
