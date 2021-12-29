//
// Created by meiyixuan on 2021-12-29.
//

#ifndef PROJECT_NEARESTPHOTONS_H
#define PROJECT_NEARESTPHOTONS_H

struct NearestPhotons {
public:
    NearestPhotons() {
        max_photons = 0;
        found_photons = 0;
        heap_full = false;
        dist_square = nullptr;
        photons = nullptr;
    }

    ~NearestPhotons() {
        delete[] dist_square;
        delete[] photons;
    }

public:
    Vector3d pos;
    int max_photons, found_photons;
    bool heap_full;
    double* dist_square;
    Photon** photons;
};

#endif //PROJECT_NEARESTPHOTONS_H
