//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_RECTANGLE_H
#define PROJECT_RECTANGLE_H

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

class XZRectangle : public Hittable {
public:
    XZRectangle() = default;

    XZRectangle(double _x0, double _x1, double _z0, double _z1, double _y, shared_ptr<Material> mat_ptr)
            : Hittable(std::move(mat_ptr)), x0(_x0), x1(_x1), z0(_z0), z1(_z1), y(_y) {};

    bool hit(const Ray &ray, double t_min, double t_max, Hit &hit) const override {
        // check if hit plane
        auto t = (y - ray.origin().y()) / ray.direction().y();
        if (t < t_min || t > t_max) return false;

        // check if hit rectangle
        auto x = ray.origin().x() + t * ray.direction().x();
        auto z = ray.origin().z() + t * ray.direction().z();
        if (x < x0 || x > x1 || z < z0 || z > z1) return false;

        // hit
        hit.u = (x - x0) / (x1 - x0);
        hit.v = (z - z0) / (z1 - z0);
        hit.t = t;
        auto outward_normal = Vector3d(0, 1, 0);
        hit.set_face_normal(ray, outward_normal);
        hit.mat_ptr = mat_ptr;
        hit.hit_point = ray.at(t);
        return true;
    }

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        output_box = AABB(Point(x0, y - 0.0001, z0), Point(x1, y + 0.0001, z1));
        return true;
    }

private:
    double x0{}, x1{}, z0{}, z1{};
    double y{};
};

class YZRectangle : public Hittable {
public:
    YZRectangle() = default;

    YZRectangle(double _y0, double _y1, double _z0, double _z1, double _x, shared_ptr<Material> mat_ptr)
            : Hittable(std::move(mat_ptr)), y0(_y0), y1(_y1), z0(_z0), z1(_z1), x(_x) {};

    bool hit(const Ray &ray, double t_min, double t_max, Hit &hit) const override {
        // check if hit plane
        auto t = (x - ray.origin().x()) / ray.direction().x();
        if (t < t_min || t > t_max) return false;

        // check if hit rectangle
        auto y = ray.origin().y() + t * ray.direction().y();
        auto z = ray.origin().z() + t * ray.direction().z();
        if (y < y0 || y > y1 || z < z0 || z > z1) return false;

        // hit
        hit.u = (y - y0) / (y1 - y0);
        hit.v = (z - z0) / (z1 - z0);
        hit.t = t;
        auto outward_normal = Vector3d(1, 0, 0);
        hit.set_face_normal(ray, outward_normal);
        hit.mat_ptr = mat_ptr;
        hit.hit_point = ray.at(t);
        return true;
    }

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        output_box = AABB(Point(x - 0.0001, y0, z0), Point(x + 0.0001, y1, z1));
        return true;
    }

private:
    double y0{}, y1{}, z0{}, z1{};
    double x{};
};

#endif //PROJECT_RECTANGLE_H
