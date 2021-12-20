#include <utility>

//
// Created by meiyixuan on 2021-12-20.
//

#ifndef PROJECT_TRIANGLE_H
#define PROJECT_TRIANGLE_H

struct Vertex {
    Vertex(double _u, double _v, Point _point, Vector3d _normal = {0, 0, 0})
            : u(_u), v(_v), point(_point), normal(_normal) {}

    double u{0};
    double v{0};
    Point point;
    Vector3d normal;
};

class Triangle : public Hittable {
public:
    Triangle() = default;

    Triangle(std::shared_ptr<Vertex> v0, std::shared_ptr<Vertex> v1, std::shared_ptr<Vertex> v2,
             std::shared_ptr<Material> m) : Hittable(std::move(m)),
                                            normal(normalize(cross(v1->point - v0->point, v2->point - v0->point))),
                                            vertices{std::move(v0), std::move(v1), std::move(v2)} {}

    bool hit(const Ray &ray, double t_min, double t_max, Hit &hit) const override {
        // initialize (Möller–Trumbore intersection algorithm)
        Vector3d edge1, edge2, h, s, q;
        double a, f, u, v;
        edge1 = vertices[1]->point - vertices[0]->point;
        edge2 = vertices[2]->point - vertices[0]->point;
        h = cross(ray.direction(), edge2);
        a = dot(edge1, h);

        // This ray is parallel to this triangle.
        if (a > -EPSILON && a < EPSILON) return false;
        f = 1.0 / a;
        s = ray.origin() - vertices[0]->point;
        u = f * dot(s, h);
        if (u < 0.0 || u > 1.0) return false;
        q = cross(s, edge1);
        v = f * dot(ray.direction(), q);
        if (v < 0.0 || u + v > 1.0) return false;

        // At this stage we can compute t to find out where the intersection point is on the line.
        double t = f * dot(edge2, q);
        if (t < t_min || t > t_max) return false;
        hit.t = t;
        hit.hit_point = ray.at(t);
        hit.normal = normal; // can also clip
        hit.set_face_normal(ray, normal);
        hit.mat_ptr = mat_ptr;
        get_triangle_uv(hit.hit_point, hit.u, hit.v);
        return true;
    }

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        // x
        double x_min = min3(vertices[0]->point.x(), vertices[1]->point.x(), vertices[2]->point.x());
        double x_max = max3(vertices[0]->point.x(), vertices[1]->point.x(), vertices[2]->point.x());
        if (x_min == x_max) {
            x_min -= 0.0001;
            x_max += 0.0001;
        }

        // y
        double y_min = min3(vertices[0]->point.y(), vertices[1]->point.y(), vertices[2]->point.y());
        double y_max = max3(vertices[0]->point.y(), vertices[1]->point.y(), vertices[2]->point.y());
        if (y_min == y_max) {
            y_min -= 0.0001;
            y_max += 0.0001;
        }

        // z
        double z_min = min3(vertices[0]->point.z(), vertices[1]->point.z(), vertices[2]->point.z());
        double z_max = max3(vertices[0]->point.z(), vertices[1]->point.z(), vertices[2]->point.z());
        if (z_min == z_max) {
            z_min -= 0.0001;
            z_max += 0.0001;
        }

        output_box = AABB({x_min, y_min, z_min}, {x_max, y_max, z_max});
        return true;
    }

private:
    Vector3d normal;
    std::shared_ptr<Vertex> vertices[3];

    inline void barycentric(const Point &p, double &u, double &v, double &w) const {
        // Compute barycentric coordinates (u, v, w) for
        // point p with respect to triangle (0, 1, 2)
        // From Christer Ericson's Real-Time Collision Detection
        Vector3d v0 = vertices[1]->point - vertices[0]->point;
        Vector3d v1 = vertices[2]->point - vertices[0]->point;
        Vector3d v2 = p - vertices[0]->point;
        double d00 = dot(v0, v0);
        double d01 = dot(v0, v1);
        double d11 = dot(v1, v1);
        double d20 = dot(v2, v0);
        double d21 = dot(v2, v1);
        double d = d00 * d11 - d01 * d01;
        v = (d11 * d20 - d01 * d21) / d;
        w = (d00 * d21 - d01 * d20) / d;
        u = 1.0 - v - w;
    }

    inline void get_triangle_uv(const Point &p, double &u, double &v) const {
        double a, b, c;
        barycentric(p, a, b, c);
        u = a * vertices[0]->u + b * vertices[1]->u + c * vertices[2]->u;
        v = a * vertices[0]->v + b * vertices[1]->v + c * vertices[2]->v;
    }

    static inline double min3(const double &a, const double &b, const double &c) {
        return fmin(fmin(a, b), c);
    }

    static inline double max3(const double &a, const double &b, const double &c) {
        return fmax(fmax(a, b), c);
    }

};

#endif //PROJECT_TRIANGLE_H
