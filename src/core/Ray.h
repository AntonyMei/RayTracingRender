//
// Created by meiyixuan on 2021-12-09.
// This file contains the definition of Ray.
//

#ifndef PROJECT_RAY_H
#define PROJECT_RAY_H

class Ray {
public:
    Ray() : o(), d(), tm{} {}

    Ray(const Point &origin, const Vector3d &direction, double time = 0.0, bool _is_camera_ray = false) :
            o(origin), d(direction), tm(time), is_camera_ray(_is_camera_ray) {}

    // utils
    Point origin() const { return o; }

    Vector3d direction() const { return d; }

    double time() const { return tm; }

    bool camera_ray() const { return is_camera_ray; }

    Point at(double t) const { return o + t * d; }

private:
    // origin and direction
    Point o;
    Vector3d d;
    double tm;
    bool is_camera_ray{false};
};


#endif //PROJECT_RAY_H
