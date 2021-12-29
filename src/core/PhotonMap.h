//
// Created by meiyixuan on 2021-12-29.
//

#ifndef PROJECT_PHOTONMAP_H
#define PROJECT_PHOTONMAP_H

int calculate_median(int start, int end);

class PhotonMap {
public:
    PhotonMap() {
        max_photon_num = 10000;
        photon_num = 0;
        box_min = Vector3d(10000, 10000, 10000);
        box_max = Vector3d(-10000, -10000, -10000);
    };

    explicit PhotonMap(int max) {
        max_photon_num = max;
        photon_num = 0;
        box_min = Vector3d(10000, 10000, 10000);
        box_max = Vector3d(-10000, -10000, -10000);
    };

    ~PhotonMap() = default;

    void store(const std::shared_ptr<Photon> &p) {
        if (photon_num >= max_photon_num) return;
        photon_num += 1;
        photon_list.push_back(p);
        box_min = Vector3d(fmin(box_min.x(), p->position.x()),
                           fmin(box_min.y(), p->position.y()),
                           fmin(box_min.z(), p->position.z()));
        box_max = Vector3d(fmax(box_max.x(), p->position.x()),
                           fmax(box_max.y(), p->position.y()),
                           fmax(box_max.z(), p->position.z()));
    }

    static void median_split(std::vector<std::shared_ptr<Photon>> temp, int start, int end, int med, int axis) {
        // heap sort
        int l = start, r = end;
        while (l < r) {
            double key = temp[r]->position[axis];
            int i = l - 1, j = r;
            while (true) {
                while (temp[++i]->position[axis] < key);
                while (temp[--j]->position[axis] < key && j > l);
                if (i >= j) break;
                std::swap(temp[i], temp[j]);
            }
            std::swap(temp[i], temp[r]);
            if (i >= med) r = i - 1;
            if (i <= med) l = i + 1;
        }
    }

    void balance() {
        auto temp = std::vector<std::shared_ptr<Photon>>(photon_num + 1);
        for (int i = 1; i <= photon_num; ++i) {
            temp[i] = photon_list[i];
        }
        balance_segment(temp, 1, 1, photon_num);
    }

    void balance_segment(std::vector<std::shared_ptr<Photon>> temp, int index, int start, int end) {
        if (start == end) {
            photon_list[index] = temp[start];
            return;
        }
        int med = calculate_median(start, end);
        int axis = -1;
        if (box_max.x() - box_min.x() > box_max.y() - box_min.y() &&
            box_max.x() - box_min.x() > box_max.z() - box_min.z())
            axis = 0;
        else if (box_max.y() - box_min.y() > box_max.z() - box_min.z())
            axis = 1;
        else
            axis = 2;
        median_split(temp, start, end, med, axis);
        photon_list[index] = temp[med];
        photon_list[index]->axis = axis;
        if (start < med) {
            double tmp = box_max[axis];
            box_max[axis] = photon_list[index]->position[axis];
            balance_segment(temp, index * 2, start, med - 1);
            box_max[axis] = tmp;
        }
        if (med < end) {
            double tmp = box_min[axis];
            box_min[axis] = photon_list[index]->position[axis];
            balance_segment(temp, index * 2 + 1, med + 1, end);
            box_min[axis] = tmp;
        }
    }

private:
    int photon_num;
    int max_photon_num;
    std::vector<std::shared_ptr<Photon>> photon_list;
    Point box_min, box_max;
};

int calculate_median(int start, int end) {
    int num = end - start + 1;
    int med;
    int as = 1, b = 2;
    while (as < num) {
        as += b;
        b *= 2;
    }
    if (as == num) return start + num / 2;
    b /= 2;
    if (as - b / 2 < num) {
        return start + as / 2;
    } else {
        return start + as / 2 - (as - b / 2 - num);
    }
}

#endif //PROJECT_PHOTONMAP_H
