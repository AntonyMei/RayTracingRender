//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_HIT_H
#define PROJECT_HIT_H

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

#endif //PROJECT_HIT_H
