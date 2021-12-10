//
// Created by meiyixuan on 2021-12-10.
//

#ifndef PROJECT_CONST_H
#define PROJECT_CONST_H

// includes
#include <cmath>
#include <limits>
#include <memory>

#include "Ray.h"
#include "Vector3d.h"

// memory
using std::shared_ptr;
using std::make_shared;

// math
using std::sqrt;
const double inf = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double deg2rad(double deg) {
    return deg * pi / 180.0;
}

#endif //PROJECT_CONST_H
