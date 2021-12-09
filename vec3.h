//
// Created by meiyixuan on 2021-12-09.
//

#ifndef PROJECT_VEC3_H
#define PROJECT_VEC3_H

#include <cmath>
#include <iostream>



class vec3 {
public:
    // constructors
    vec3() : val{0, 0, 0} {}

    vec3(double x, double y, double z) : val{x, y, z} {}

    // data
    double x() const { return val[0]; }

    double y() const { return val[1]; }

    double z() const { return val[2]; }

    // operators
    vec3 &operator+=(const vec3 &v) {
        val[0] += v.val[0];
        val[1] += v.val[1];
        val[2] += v.val[2];
        return *this;
    }

    vec3 &operator-=(const vec3 &v) {
        val[0] -= v.val[0];
        val[1] -= v.val[1];
        val[2] -= v.val[2];
        return *this;
    }

    vec3 &operator*=(const double t) {
        val[0] *= t;
        val[1] *= t;
        val[2] *= t;
        return *this;
    }

    vec3 &operator/=(const double t) {
        return *this *= 1 / t;
    }

    vec3 operator-() const { return {-val[0], -val[1], -val[2]}; }

    double operator[](int i) const { return val[i]; }

    double &operator[](int i) { return val[i]; }

    double length() const {
        return std::sqrt(length_squared());
    }

    double length_squared() const {
        return val[0] * val[0] + val[1] * val[1] + val[2] * val[2];
    }

private:

    double val[3];

    // for access to val directly
    friend inline std::ostream &operator<<(std::ostream &out, const vec3 &v);

    friend inline vec3 operator+(const vec3 &u, const vec3 &v);

    friend inline vec3 operator-(const vec3 &u, const vec3 &v);

    friend inline vec3 operator*(const vec3 &u, const vec3 &v);

    friend inline vec3 operator*(double t, const vec3 &v);

    friend inline double dot(const vec3 &u, const vec3 &v);

    friend inline vec3 cross(const vec3 &u, const vec3 &v);
};

using point3 = vec3;   // 3D point
using color = vec3;    // RGB color

inline std::ostream &operator<<(std::ostream &out, const vec3 &v) {
    return out << v.val[0] << ' ' << v.val[1] << ' ' << v.val[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return {u.val[0] + v.val[0], u.val[1] + v.val[1], u.val[2] + v.val[2]};
}

inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return {u.val[0] - v.val[0], u.val[1] - v.val[1], u.val[2] - v.val[2]};
}

inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return {u.val[0] * v.val[0], u.val[1] * v.val[1], u.val[2] * v.val[2]};
}

inline vec3 operator*(double t, const vec3 &v) {
    return {t * v.val[0], t * v.val[1], t * v.val[2]};
}

inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator/(vec3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) {
    return u.val[0] * v.val[0] + u.val[1] * v.val[1] + u.val[2] * v.val[2];
}

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return {u.val[1] * v.val[2] - u.val[2] * v.val[1],
            u.val[2] * v.val[0] - u.val[0] * v.val[2],
            u.val[0] * v.val[1] - u.val[1] * v.val[0]};
}

inline vec3 normalize(vec3 v) {
    return v / v.length();

#endif //PROJECT_VEC3_H
