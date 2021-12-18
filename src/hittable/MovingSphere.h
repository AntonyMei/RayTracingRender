//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_MOVINGSPHERE_H
#define PROJECT_MOVINGSPHERE_H

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

#endif //PROJECT_MOVINGSPHERE_H
