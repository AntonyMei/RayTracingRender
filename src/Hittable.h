//
// Created by meiyixuan on 2021-12-10.
//

#ifndef PROJECT_HITTABLE_H
#define PROJECT_HITTABLE_H

#include "Ray.h"
#include <cmath>

struct Hit {
    Point hit_point;
    Vector3d normal;
    double t{0};
    bool front_face{false};

    inline void set_face_normal(const Ray& r, const Vector3d& n) {
        front_face = (dot(r.direction(), n) < 0);
        normal = front_face ? n : -n;
    }
};

// base class for all hittable objects
class Hittable {
public:
    virtual bool hit(const Ray &r, double t_min, double t_max, Hit &rec) const = 0;
};

class Sphere : public Hittable {
public:
    // constructors
    Sphere() : r{0} {}

    Sphere(Point center, double radius) : c{center}, r{radius} {};

    // utilities
    Point center() const { return c; }

    double radius() const { return r; }

    bool hit(const Ray &ray, double t_min, double t_max, Hit &hit) const override {
        // calculate
        Vector3d oc = ray.origin() - c;
        auto _a = ray.direction().squared_length();
        auto _b = dot(oc, ray.direction());
        auto _c = oc.squared_length() - r * r;
        auto discriminant = _b * _b - _a * _c;
        if (discriminant < 0) return false;
        auto sqrt_d = sqrt(discriminant);

        // fill hit
        auto root = (-_b - sqrt_d) / _a;
        if (root > t_min && root < t_max) {
            // small root is fine
            hit.t = root;
            hit.hit_point = ray.at(root);
            Vector3d normal = (hit.hit_point - c) / r;
            hit.set_face_normal(ray, normal);
            return true;
        } else {
            // may use larger root
            root = (-_b + sqrt_d) / _a;
            if (root > t_min && root < t_max) {
                hit.t = root;
                hit.hit_point = ray.at(root);
                Vector3d normal = (hit.hit_point - c) / r;
                hit.set_face_normal(ray, normal);
                return true;
            } else { return false; }
        }
    }

private:
    Point c;
    double r;
};

#endif //PROJECT_HITTABLE_H
