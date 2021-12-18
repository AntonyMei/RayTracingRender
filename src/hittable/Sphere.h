//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_SPHERE_H
#define PROJECT_SPHERE_H

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

#endif //PROJECT_SPHERE_H
