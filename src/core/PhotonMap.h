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
        photon_list = new Photon[10000];
        box_min = Vector3d(10000, 10000, 10000);
        box_max = Vector3d(-10000, -10000, -10000);
    };

    explicit PhotonMap(int max) {
        max_photon_num = max;
        photon_num = 0;
        photon_list = new Photon[max];
        box_min = Vector3d(10000, 10000, 10000);
        box_max = Vector3d(-10000, -10000, -10000);
    };

    ~PhotonMap() = default;

    void store(Photon p) {
        if (photon_num >= max_photon_num) return;
        photon_list[photon_num++] = p;
        box_min = Vector3d(fmin(box_min.x(), p.position.x()),
                           fmin(box_min.y(), p.position.y()),
                           fmin(box_min.z(), p.position.z()));
        box_max = Vector3d(fmax(box_max.x(), p.position.x()),
                           fmax(box_max.y(), p.position.y()),
                           fmax(box_max.z(), p.position.z()));
    }

    static void median_split(Photon *temp, int start, int end, int med, int axis) {
        // heap sort
        int l = start, r = end;
        while (l < r) {
            double key = temp[r].position[axis];
            int i = l - 1, j = r;
            while (true) {
                while (temp[++i].position[axis] < key);
                while (temp[--j].position[axis] > key && j > l);
                if (i >= j) break;
                std::swap(temp[i], temp[j]);
            }
            std::swap(temp[i], temp[r]);
            if (i >= med) r = i - 1;
            if (i <= med) l = i + 1;
        }
    }

    void balance() {
        auto *temp = new Photon[photon_num + 1];
        for (int i = 1; i <= photon_num; ++i) {
            temp[i] = photon_list[i];
        }
        balance_segment(temp, 1, 1, photon_num);
        delete[] temp;
    }

    void balance_segment(Photon *temp, int index, int start, int end) {
        if (start == end) {
            photon_list[index] = temp[start];
            return;
        }
        int med = calculate_median(start, end);
        int axis;
        if (box_max.x() - box_min.x() > box_max.y() - box_min.y() &&
            box_max.x() - box_min.x() > box_max.z() - box_min.z())
            axis = 0;
        else if (box_max.y() - box_min.y() > box_max.z() - box_min.z())
            axis = 1;
        else
            axis = 2;
        median_split(temp, start, end, med, axis);
        photon_list[index] = temp[med];
        photon_list[index].axis = axis;
        if (start < med) {
            double tmp = box_max[axis];
            box_max[axis] = photon_list[index].position[axis];
            balance_segment(temp, index * 2, start, med - 1);
            box_max[axis] = tmp;
        }
        if (med < end) {
            double tmp = box_min[axis];
            box_min[axis] = photon_list[index].position[axis];
            balance_segment(temp, index * 2 + 1, med + 1, end);
            box_min[axis] = tmp;
        }
    }

    void get_nearest_photons(NearestPhotons *nearest_photons, int index) {
        if (index > photon_num) return;
        Photon *photon = &photon_list[index];
        if (index * 2 <= photon_num) {
            double dist = nearest_photons->pos[photon->axis] - photon->position[photon->axis];
            if (dist < 0) {
                get_nearest_photons(nearest_photons, index * 2);
                if (dist * dist < nearest_photons->dist_square[0])
                    get_nearest_photons(nearest_photons, index * 2 + 1);
            } else {
                get_nearest_photons(nearest_photons, index * 2 + 1);
                if (dist * dist < nearest_photons->dist_square[0])
                    get_nearest_photons(nearest_photons, index * 2);
            }
        }
        double dist_square = (photon->position - nearest_photons->pos).squared_length();
        if (dist_square > nearest_photons->dist_square[0]) return;
        if (nearest_photons->found_photons < nearest_photons->max_photons) {
            nearest_photons->found_photons++;
            nearest_photons->dist_square[nearest_photons->found_photons] = dist_square;
            nearest_photons->photons[nearest_photons->found_photons] = photon;
        } else {
            if (!nearest_photons->heap_full) {
                for (int i = nearest_photons->found_photons >> 1; i >= 1; --i) {
                    int par = i;
                    auto temp_photon = nearest_photons->photons[i];
                    double temp_dist_square = nearest_photons->dist_square[i];
                    while ((par << 1) <= nearest_photons->found_photons) {
                        int j = par << 1;
                        if (j + 1 <= nearest_photons->found_photons
                            && nearest_photons->dist_square[j] < nearest_photons->dist_square[j + 1])
                            j++;
                        if (temp_dist_square >= nearest_photons->dist_square[j]) break;
                        nearest_photons->photons[par] = nearest_photons->photons[j];
                        nearest_photons->dist_square[par] = nearest_photons->dist_square[j];
                        par = j;
                    }
                    nearest_photons->photons[par] = temp_photon;
                    nearest_photons->dist_square[par] = temp_dist_square;
                }
                nearest_photons->heap_full = true;
            }
            int par = 1;
            while ((par << 1) <= nearest_photons->found_photons) {
                int j = par << 1;
                if (j + 1 <= nearest_photons->found_photons
                    && nearest_photons->dist_square[j] < nearest_photons->dist_square[j + 1])
                    j++;
                if (dist_square >= nearest_photons->dist_square[j]) break;
                nearest_photons->photons[par] = nearest_photons->photons[j];
                nearest_photons->dist_square[par] = nearest_photons->dist_square[j];
                par = j;
            }
            nearest_photons->photons[par] = photon;
            nearest_photons->dist_square[par] = dist_square;
            nearest_photons->dist_square[0] = nearest_photons->dist_square[1];
        }
    }

    Color get_irradiance(Vector3d pos, Vector3d norm, double max_dist, int N) {
        Vector3d ret(0, 0, 0);
        NearestPhotons np;
        np.pos = pos;
        np.max_photons = N;
        np.dist_square = new double[N + 1];
        np.photons = new Photon *[N + 1];
        np.dist_square[0] = max_dist * max_dist;
        get_nearest_photons(&np, 1);
        if (np.found_photons <= 10) return ret;
        for (int i = 1; i <= np.found_photons; ++i) {
            auto dir = np.photons[i]->direction;
            // filtering
            double filter = 1.0 - (pos - np.photons[i]->position).length() / (1.1 * max_dist);
            if (dot(norm, dir) < 0) ret = ret + np.photons[i]->power * filter;
        }
        ret = ret * (1.0 / (2000000 * pi * np.dist_square[0]));
        return ret;
    }

    int get_photon_num() const { return photon_num; }

private:
    int photon_num;
    int max_photon_num;
    Photon *photon_list;
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
