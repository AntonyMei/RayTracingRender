//
// Created by meiyixuan on 2021-12-10.
//

#ifndef PROJECT_UTILS_H
#define PROJECT_UTILS_H

// includes
#include <cmath>
#include <limits>
#include <memory>
#include <vector>
#include <random>

#include "Ray.h"
#include "Vector3d.h"
#include "Accelerator.h"
#include "Camera.h"

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

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// random
inline double random_double() {
    // random number in [0, 1)
    static std::random_device rd;
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator(rd());
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // random real in [min,max).
    return min + (max - min) * random_double();
}

#endif //PROJECT_UTILS_H
