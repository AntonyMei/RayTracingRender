//
// Created by meiyixuan on 2021-12-29.
//

#ifndef PROJECT_PHOTONLIGHT_H
#define PROJECT_PHOTONLIGHT_H

class YZRectangleLight : public Hittable {
public:
    YZRectangleLight() = default;

    YZRectangleLight(double _y0, double _y1, double _z0, double _z1, double _x, shared_ptr<Material> mat_ptr)
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

    void generate_photon(Vector3d origin, Vector3d &direction, double &power, Hit &hit) const {
        origin = Vector3d(y0 + random_double() * (y1 - y0), x, z0 + random_double() * (z1 - z0));
        direction = random_in_hemisphere(hit.normal);
        power = dot(direction, hit.normal);
    }

private:
    double y0{}, y1{}, z0{}, z1{};
    double x{};
};

#endif //PROJECT_PHOTONLIGHT_H
