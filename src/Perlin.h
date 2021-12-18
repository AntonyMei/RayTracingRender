//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_PERLIN_H
#define PROJECT_PERLIN_H

class PerlinNoise {
public:
    PerlinNoise() {
        rand_double = new double[point_count];
        for (int i = 0; i < point_count; ++i) {
            rand_double[i] = random_double_fixed();
        }

        perm_x = perlin_generate_perm();
        perm_y = perlin_generate_perm();
        perm_z = perlin_generate_perm();
    }

    ~PerlinNoise() {
        delete[] rand_double;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }

    double noise(const Point &p) const {
        auto i = static_cast<int>(4 * p.x()) & 255;
        auto j = static_cast<int>(4 * p.y()) & 255;
        auto k = static_cast<int>(4 * p.z()) & 255;

        return rand_double[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
    }

private:
    static const int point_count = 256;
    double *rand_double;
    int *perm_x;
    int *perm_y;
    int *perm_z;

    static int *perlin_generate_perm() {
        auto p = new int[point_count];

        for (int i = 0; i < PerlinNoise::point_count; i++)
            p[i] = i;

        permute(p, point_count);

        return p;
    }

    static void permute(int *p, int n) {
        for (int i = n - 1; i > 0; i--) {
            int target = random_int_fixed(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }
};

#endif //PROJECT_PERLIN_H
