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
    double u{0};
    double v{0};
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

    virtual bool bounding_box(double time0, double time1, AABB &output_box) const = 0;

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
            get_sphere_uv(normal, hit.u, hit.v);
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
                get_sphere_uv(normal, hit.u, hit.v);
                return true;
            } else { return false; }
        }
    }

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        output_box = AABB(c - Vector3d(r, r, r), c + Vector3d(r, r, r));
        return true;
    }

private:
    Point c;
    double r;

    static void get_sphere_uv(const Point &outward_normal, double &u, double &v) {
        // outward_normal: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.

        // calculate parameter
        auto theta = acos(-outward_normal.y());
        auto phi = atan2(-outward_normal.z(), outward_normal.x()) + pi;

        // calculate uv
        u = phi / (2 * pi);
        v = theta / pi;
    }
};

class MovingSphere : public Hittable {
public:
    MovingSphere() = default;

    MovingSphere(Point cen0, Point cen1, double _time0, double _time1, double r, const shared_ptr<Material> &m)
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

    bool bounding_box(double _time0, double _time1, AABB &output_box) const override {
        AABB box0(center(_time0) - Vector3d(radius, radius, radius),
                  center(_time0) + Vector3d(radius, radius, radius));
        AABB box1(center(_time1) - Vector3d(radius, radius, radius),
                  center(_time1) + Vector3d(radius, radius, radius));
        output_box = surrounding_box(box0, box1);
        return true;
    }

    Point center(double time) const {
        return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
    }

public:
    Point center0, center1;
    double time0{}, time1{};
    double radius{};
};

class XYRectangle : public Hittable {
public:
    XYRectangle() = default;

    XYRectangle(double _x0, double _x1, double _y0, double _y1, double _z, std::shared_ptr<Material> mat_ptr)
            : Hittable(std::move(mat_ptr)), x0(_x0), x1(_x1), y0(_y0), y1(_y1), z(_z) {}

    bool hit(const Ray &ray, double t_min, double t_max, Hit &hit) const override {
        // check if hit plane
        auto t = (z - ray.origin().z()) / ray.direction().z();
        if (t < t_min || t > t_max) return false;

        // check if hit rect
        auto x = ray.origin().x() + t * ray.direction().x();
        auto y = ray.origin().y() + t * ray.direction().y();
        if (x < x0 || x > x1 || y < y0 || y > y1) return false;

        // hit
        hit.u = (x - x0) / (x1 - x0);
        hit.v = (y - y0) / (y1 - y0);
        hit.t = t;
        auto outward_normal = Vector3d(0, 0, 1);
        hit.set_face_normal(ray, outward_normal);
        hit.mat_ptr = mat_ptr;
        hit.hit_point = ray.at(t);
        return true;
    }

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        output_box = AABB(Point(x0, y0, z - 0.0001), Point(x1, y1, z + 0.0001));
        return true;
    }

private:
    double x0{}, x1{}, y0{}, y1{};
    double z{};
};

#endif //PROJECT_HITTABLE_H
