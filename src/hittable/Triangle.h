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
             std::shared_ptr<Material> m, std::shared_ptr<BumpMaterial> b = nullptr)
            : Hittable(std::move(m)), bump_ptr(std::move(b)),
              normal(normalize(cross(v1->point - v0->point, v2->point - v0->point))),
              vertices{std::move(v0), std::move(v1), std::move(v2)} {
        // calculate normal
        if (vertices[0]->normal.length() <= EPSILON &&
            vertices[1]->normal.length() <= EPSILON &&
            vertices[2]->normal.length() <= EPSILON) {
            use_computed_normal = true;
        }
        // calculate uv to xyz map
        map_uv_2_xyz();
    }

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
        // basic hit information
        double t = f * dot(edge2, q);
        if (t < t_min || t > t_max) return false;
        hit.t = t;
        hit.hit_point = ray.at(t);
        hit.mat_ptr = mat_ptr;
        // set normal
        double w0, w1, w2;
        barycentric(hit.hit_point, w0, w1, w2);
        if (!use_computed_normal)
            hit.normal = normalize(w0 * vertices[0]->normal +
                                   w1 * vertices[1]->normal +
                                   w2 * vertices[2]->normal);
        else
            hit.normal = normal;
        // add bump map when available
        if (bump_ptr) {
            // get perturbation
            auto perturb0 = bump_ptr->get_normal(vertices[0]->u, vertices[0]->v);
            auto perturb1 = bump_ptr->get_normal(vertices[1]->u, vertices[1]->v);
            auto perturb2 = bump_ptr->get_normal(vertices[2]->u, vertices[2]->v);
            auto perturb = w0 * perturb0 + w1 * perturb1 + w2 * perturb2;
            hit.normal = normalize(hit.normal + perturb[0] * u_vec + perturb[1] * v_vec);
        }
        hit.set_face_normal(ray, normal);
        // set u v
        hit.u = w0 * vertices[0]->u + w1 * vertices[1]->u + w2 * vertices[2]->u;
        hit.v = w0 * vertices[0]->v + w1 * vertices[1]->v + w2 * vertices[2]->v;
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
    bool use_computed_normal{false};
    Vector3d normal;
    std::shared_ptr<Vertex> vertices[3];
    std::shared_ptr<BumpMaterial> bump_ptr;
    // unit vector of u, v in xyz space
    Vector3d u_vec, v_vec;

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

    inline void map_uv_2_xyz() {
        // 1 wrt 0
        double a1 = vertices[1]->u - vertices[0]->u;
        double a2 = vertices[1]->v - vertices[0]->v;
        Vector3d a_xyz = vertices[1]->point - vertices[0]->point;
        // 2 wrt 0
        double b1 = vertices[2]->u - vertices[0]->u;
        double b2 = vertices[2]->v - vertices[0]->v;
        Vector3d b_xyz = vertices[2]->point - vertices[0]->point;
        // denominator and check for degradation
        double denominator = a1 * b2 - a2 * b1;
        if (fabs(denominator) < EPSILON) {
            bool recover_flag = true;
            // no uv coordinate case
            if (a1 == 0 && a2 == 0 && b1 == 0 && b2 == 0) return;
            // u degradation
            if (vertices[0]->u == vertices[1]->u && vertices[0]->u == vertices[2]->u) {
                double v_min = min3(vertices[0]->v, vertices[1]->v, vertices[2]->v);
                double v_max = max3(vertices[0]->v, vertices[1]->v, vertices[2]->v);
                if (v_min == v_max) recover_flag = false;
                else {
                    // we can still recover v in this case
                    int max_id = v_max == vertices[0]->v ? 0
                                                         : v_max == vertices[1]->v ? 1 : 2;
                    int min_id = v_min == vertices[0]->v ? 0
                                                         : v_min == vertices[1]->v ? 1 : 2;
                    double v_scale = 1 / (v_max - v_min);
                    v_vec = (vertices[max_id]->point - vertices[min_id]->point) * v_scale;
                    u_vec = Vector3d();
                    u_degrade_triangles += 1;
                    return;
                }
            }
            // v degradation
            if (vertices[0]->v == vertices[1]->v && vertices[0]->v == vertices[2]->v) {
                double u_min = min3(vertices[0]->u, vertices[1]->u, vertices[2]->u);
                double u_max = max3(vertices[0]->u, vertices[1]->u, vertices[2]->u);
                if (u_min == u_max) recover_flag = false;
                else {
                    // we can still recover u in this case
                    int max_id = u_max == vertices[0]->u ? 0
                                                         : u_max == vertices[1]->u ? 1 : 2;
                    int min_id = u_min == vertices[0]->u ? 0
                                                         : u_min == vertices[1]->u ? 1 : 2;
                    double u_scale = 1 / (u_max - u_min);
                    v_vec = Vector3d();
                    u_vec = (vertices[max_id]->point - vertices[min_id]->point) * u_scale;
                    v_degrade_triangles += 1;
                    return;
                }
            }
            // unable to recover and log
//            std::cerr << "Failed to map uv to xyz" << std::endl;
//            if (random_double() < 1) {
//                std::cerr << vertices[0]->u << "     " << vertices[0]->v << std::endl;
//                std::cerr << vertices[1]->u << "     " << vertices[1]->v << std::endl;
//                std::cerr << vertices[1]->u << "     " << vertices[1]->v << std::endl;
//                std::cerr << std::endl;
//            }
            u_vec = Vector3d();
            v_vec = Vector3d();
            unrecoverable_triangles += 1;
            return;
        }
        // calculate for u
        double ux = b2 / denominator;
        double uy = -a2 / denominator;
        u_vec = ux * a_xyz + uy * b_xyz;
        // calculate for v
        double vx = -b1 / denominator;
        double vy = a1 / denominator;
        v_vec = vx * a_xyz + vy * b_xyz;
        normal_triangles += 1;
    }

    static inline double min3(const double &a, const double &b, const double &c) {
        return fmin(fmin(a, b), c);
    }

    static inline double max3(const double &a, const double &b, const double &c) {
        return fmax(fmax(a, b), c);
    }

};

#endif //PROJECT_TRIANGLE_H
