//
// Created by meiyixuan on 2021-12-29.
//

#ifndef PROJECT_PHOTONMAP_H
#define PROJECT_PHOTONMAP_H

class PhotonMap {
public:
    PhotonMap() = default;

    ~PhotonMap() = default;

    void store(const std::shared_ptr<Photon>& p) {
        photon_list.push_back(p);
    }

private:
    std::vector<std::shared_ptr<Photon>> photon_list;
    Point box_min, box_max;
};

#endif //PROJECT_PHOTONMAP_H
