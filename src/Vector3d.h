//
// Created by meiyixuan on 2021-12-09.
// Basic math class for vector operations.
//

#ifndef PROJECT_VECTOR3D_H
#define PROJECT_VECTOR3D_H

#include <cmath>
#include <iostream>

class Vector3d {
public:
    // constructors
    Vector3d() : val{0, 0, 0} {}

    Vector3d(double x, double y, double z) : val{x, y, z} {}

    // data
    double x() const { return val[0]; }

    double y() const { return val[1]; }

    double z() const { return val[2]; }

    // operators
    Vector3d &operator+=(const Vector3d &v) {
        val[0] += v.val[0];
        val[1] += v.val[1];
        val[2] += v.val[2];
        return *this;
    }

    Vector3d &operator-=(const Vector3d &v) {
        val[0] -= v.val[0];
        val[1] -= v.val[1];
        val[2] -= v.val[2];
        return *this;
    }

    Vector3d &operator*=(const double t) {
        val[0] *= t;
        val[1] *= t;
        val[2] *= t;
        return *this;
    }

    Vector3d &operator/=(const double t) {
        return *this *= 1 / t;
    }

    Vector3d operator-() const { return {-val[0], -val[1], -val[2]}; }

    double operator[](int i) const { return val[i]; }

    double &operator[](int i) { return val[i]; }

    double length() const {
        return std::sqrt(squared_length());
    }

    double squared_length() const {
        return val[0] * val[0] + val[1] * val[1] + val[2] * val[2];
    }

    // utils
    inline static Vector3d random() {
        return Vector3d(random_double(), random_double(), random_double());
    }


private:

    double val[3];

    // for access to val directly
    friend inline std::ostream &operator<<(std::ostream &out, const Vector3d &v);

    friend inline Vector3d operator+(const Vector3d &u, const Vector3d &v);

    friend inline Vector3d operator-(const Vector3d &u, const Vector3d &v);

    friend inline Vector3d operator*(const Vector3d &u, const Vector3d &v);

    friend inline Vector3d operator*(double t, const Vector3d &v);

    friend inline double dot(const Vector3d &u, const Vector3d &v);

    friend inline Vector3d cross(const Vector3d &u, const Vector3d &v);
};

using Point = Vector3d;   // 3D point
using Color = Vector3d;    // RGB color

inline std::ostream &operator<<(std::ostream &out, const Vector3d &v) {
    return out << v.val[0] << ' ' << v.val[1] << ' ' << v.val[2];
}

inline Vector3d operator+(const Vector3d &u, const Vector3d &v) {
    return {u.val[0] + v.val[0], u.val[1] + v.val[1], u.val[2] + v.val[2]};
}

inline Vector3d operator-(const Vector3d &u, const Vector3d &v) {
    return {u.val[0] - v.val[0], u.val[1] - v.val[1], u.val[2] - v.val[2]};
}

inline Vector3d operator*(const Vector3d &u, const Vector3d &v) {
    return {u.val[0] * v.val[0], u.val[1] * v.val[1], u.val[2] * v.val[2]};
}

inline Vector3d operator*(double t, const Vector3d &v) {
    return {t * v.val[0], t * v.val[1], t * v.val[2]};
}

inline Vector3d operator*(const Vector3d &v, double t) {
    return t * v;
}

inline Vector3d operator/(Vector3d v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vector3d &u, const Vector3d &v) {
    return u.val[0] * v.val[0] + u.val[1] * v.val[1] + u.val[2] * v.val[2];
}

inline Vector3d cross(const Vector3d &u, const Vector3d &v) {
    return {u.val[1] * v.val[2] - u.val[2] * v.val[1],
            u.val[2] * v.val[0] - u.val[0] * v.val[2],
            u.val[0] * v.val[1] - u.val[1] * v.val[0]};
}

inline Vector3d normalize(Vector3d v) {
    return v / v.length();
}

#endif //PROJECT_VECTOR3D_H
