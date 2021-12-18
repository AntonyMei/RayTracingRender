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


// memory
using std::shared_ptr;
using std::make_shared;

// math
using std::sqrt;
const double inf = std::numeric_limits<double>::infinity();
const double TMIN = 0.001;
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

inline double random_double_fixed() {
    // random number in [0, 1), with fixed initial seed
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // random real in [min,max).
    return min + (max - min) * random_double();
}

inline double random_double_fixed(double min, double max) {
    // random real in [min,max).
    return min + (max - min) * random_double_fixed();
}

inline int random_int(int min, int max) {
    // Returns a random integer in [min,max].
    return static_cast<int>(random_double(min, max + 1));
}

// input
int parse_int(const char *buf, size_t len) {
    int res = 0;
    for (size_t i = 0; i < len; ++i) {
        res *= 10;
        res += buf[i] - '0';
    }
    return res;
}

#endif //PROJECT_UTILS_H
