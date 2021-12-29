//
// Created by meiyixuan on 2021-12-29.
//

#ifndef PROJECT_PHOTON_H
#define PROJECT_PHOTON_H

struct Photon {
public:
    Photon() = default;

    Photon(Vector3d pos, Vector3d dir, Vector3d _power) : position(pos), direction(dir), power(_power) {}

public:
    Vector3d position;
    Vector3d direction;
    Vector3d power; // in color
    int axis{0};
};

#endif //PROJECT_PHOTON_H
