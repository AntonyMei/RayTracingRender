//
// Created by meiyixuan on 2021-12-09.
// This file contains the definition of Ray.
//

#ifndef PROJECT_RAY_H
#define PROJECT_RAY_H

class Ray {
public:
    Ray() : o(), d() {}

    Ray(const Point &origin, const Vector3d &direction) : o(origin), d(direction) {}

    // utils
    Point origin() const { return o; }

    Vector3d direction() const { return d; }

    Point at(double t) const { return o + t * d; }

private:
    // origin and direction
    Point o;
    Vector3d d;
};


#endif //PROJECT_RAY_H
