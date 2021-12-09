//
// Created by meiyixuan on 2021-12-09.
//

#ifndef PROJECT_RAY_H
#define PROJECT_RAY_H

#include "Vector3d.h"

class Ray {
public:
    Ray() : o(), d() {}

    Ray(const Point3 &origin, const Vector3d &direction) : o(origin), d(direction) {}

    // utils
    Point3 origin() const { return o; }

    Vector3d direction() const { return d; }

    Point3 at(double t) const { return o + t * d; }

private:
    // origin and direction
    Point3 o;
    Vector3d d;
};


#endif //PROJECT_RAY_H
