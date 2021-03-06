//
// Created by meiyixuan on 2021-12-11.
//

#ifndef PROJECT_MATERIAL_H
#define PROJECT_MATERIAL_H

class Hit;

class Material {
public:
    virtual bool scatter(const Ray &ray_in, Hit &hit, Ray &scattered_ray) const = 0;

    virtual Color brdf(const Ray &ray_in, const Ray &ray_out, const Hit &hit) const = 0;

    virtual Color emit(double u, double v, const Point &p) const {
        return {0, 0, 0};
    };

};

#endif //PROJECT_MATERIAL_H
