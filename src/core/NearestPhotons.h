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
    }

public:
    Vector3d pos;
    int max_photons, found_photons;
    bool heap_full;
    std::vector<double> dist_square;
    std::vector<std::shared_ptr<Photon>> photons;
};

#endif //PROJECT_NEARESTPHOTONS_H
