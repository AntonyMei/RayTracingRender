//
// Created by meiyixuan on 2021-12-10.
// This file contains all hittable objects.
//

#ifndef PROJECT_HITTABLE_H
#define PROJECT_HITTABLE_H

#include <cmath>
#include <utility>

class Material;

struct Hit {
    Point hit_point;
    Vector3d normal;
    std::shared_ptr<Material> mat_ptr;
    double t{0};
    bool front_face{false};

    inline void set_face_normal(const Ray &r, const Vector3d &n) {
        front_face = (dot(r.direction(), n) < 0);
        normal = front_face ? n : -n;
    }
};

// base class for all hittable objects
class Hittable {
public:
    Hittable() : mat_ptr(nullptr) {}

    explicit Hittable(std::shared_ptr<Material> m) : mat_ptr(std::move(m)) {}

    virtual bool hit(const Ray &ray, double t_min, double t_max, Hit &hit) const = 0;

protected:
    shared_ptr<Material> mat_ptr;
};

class Sphere : public Hittable {
public:
    // constructors
    Sphere() : r{0} {}

    Sphere(Point center, double radius, std::shared_ptr<Material> m) : Hittable(std::move(m)),
                                                                       c{center}, r{radius} {};

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

        // fill intersect
        auto root = (-_b - sqrt_d) / _a;
        if (root > t_min && root < t_max) {
            // small root is fine
            hit.t = root;
            hit.hit_point = ray.at(root);
            Vector3d normal = (hit.hit_point - c) / r;
            hit.set_face_normal(ray, normal);
            hit.mat_ptr = mat_ptr;
            return true;
        } else {
            // may use larger root
            root = (-_b + sqrt_d) / _a;
            if (root > t_min && root < t_max) {
                hit.t = root;
                hit.hit_point = ray.at(root);
                Vector3d normal = (hit.hit_point - c) / r;
                hit.set_face_normal(ray, normal);
                hit.mat_ptr = mat_ptr;
                return true;
            } else { return false; }
        }
    }

private:
    Point c;
    double r;
};

class MovingSphere : public Hittable {
public:
    MovingSphere() = default;

    MovingSphere(Point cen0, Point cen1, double _time0, double _time1, double r, const shared_ptr<Material>& m)
            : Hittable(m), center0(cen0), center1(cen1), time0(_time0), time1(_time1), radius(r) {};

    bool hit(const Ray &ray, double t_min, double t_max, Hit &hit) const override {
        // calculate
        Vector3d oc = ray.origin() - center(ray.time());
        auto _a = ray.direction().squared_length();
        auto _b = dot(oc, ray.direction());
        auto _c = oc.squared_length() - radius * radius;
        auto discriminant = _b * _b - _a * _c;
        if (discriminant < 0) return false;
        auto sqrt_d = sqrt(discriminant);

        // fill intersect
        auto root = (-_b - sqrt_d) / _a;
        if (root > t_min && root < t_max) {
            // small root is fine
            hit.t = root;
            hit.hit_point = ray.at(root);
            Vector3d normal = (hit.hit_point - center(ray.time())) / radius;
            hit.set_face_normal(ray, normal);
            hit.mat_ptr = mat_ptr;
            return true;
        } else {
            // may use larger root
            root = (-_b + sqrt_d) / _a;
            if (root > t_min && root < t_max) {
                hit.t = root;
                hit.hit_point = ray.at(root);
                Vector3d normal = (hit.hit_point - center(ray.time())) / radius;
                hit.set_face_normal(ray, normal);
                hit.mat_ptr = mat_ptr;
                return true;
            } else { return false; }
        }
    }

    Point center(double time) const {
        return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
    }

public:
    Point center0, center1;
    double time0{}, time1{};
    double radius{};
};

#endif //PROJECT_HITTABLE_H
