//
// Created by meiyixuan on 2021-12-19.
//

#ifndef PROJECT_ORTHONORMALBASIS_H
#define PROJECT_ORTHONORMALBASIS_H

class OrthonormalBasis {
public:
    OrthonormalBasis() = default;

    inline Vector3d operator[](int i) const { return axis[i]; }

    Vector3d u() const { return axis[0]; }

    Vector3d v() const { return axis[1]; }

    Vector3d w() const { return axis[2]; }

    Vector3d local(double a, double b, double c) const {
        return a * u() + b * v() + c * w();
    }

    Vector3d local(const Vector3d &a) const {
        return a.x() * u() + a.y() * v() + a.z() * w();
    }

    void build_from_w(const Vector3d &n) {
        axis[2] = normalize(n);
        Vector3d a = (fabs(w().x()) > 0.9) ? Vector3d(0, 1, 0) : Vector3d(1, 0, 0);
        axis[1] = normalize(cross(w(), a));
        axis[0] = cross(w(), v());
    }

public:
    Vector3d axis[3];
};

#endif //PROJECT_ORTHONORMALBASIS_H