//
// Created by meiyixuan on 2021-12-18.
// Perlin noise generator uses code from Peter Shirley.
// Warning: Perlin noise generator must have identical seeds across all processes.
//

#ifndef PROJECT_PERLIN_H
#define PROJECT_PERLIN_H

class PerlinNoise {
public:
    PerlinNoise() {
        rand_vector = new Vector3d[point_count];
        for (int i = 0; i < point_count; ++i) {
            rand_vector[i] = normalize(Vector3d::random_fixed(-1, 1));
        }

        perm_x = perlin_generate_perm();
        perm_y = perlin_generate_perm();
        perm_z = perlin_generate_perm();
    }

    ~PerlinNoise() {
        delete[] rand_vector;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }

    double turbulence(const Point &p, int sample_depth = 7) const {
        // initialize
        auto sum = 0.0;
        auto cur_point = p;
        auto weight = 1.0;

        // weighted sum of samples
        for (int i = 0; i < sample_depth; i++) {
            sum += weight * noise(cur_point);
            weight *= 0.5;
            cur_point *= 2;
        }
        return fabs(sum);
    }

    double noise(const Point &p) const {
        // only one sample from perlin noise
        auto u = p.x() - floor(p.x());
        auto v = p.y() - floor(p.y());
        auto w = p.z() - floor(p.z());
        auto i = static_cast<int>(floor(p.x()));
        auto j = static_cast<int>(floor(p.y()));
        auto k = static_cast<int>(floor(p.z()));
        Vector3d c[2][2][2];

        for (int di = 0; di < 2; di++)
            for (int dj = 0; dj < 2; dj++)
                for (int dk = 0; dk < 2; dk++)
                    c[di][dj][dk] = rand_vector[
                            perm_x[(i + di) & 255] ^
                            perm_y[(j + dj) & 255] ^
                            perm_z[(k + dk) & 255]];

        // tri-linear interpolation
        return perlin_interpolation(c, u, v, w);
    }

private:
    static const int point_count = 256;
    Vector3d *rand_vector;
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

    static double perlin_interpolation(Vector3d c[2][2][2], double u, double v, double w) {
        // hermite smoothing to eliminate Mach Band
        auto uu = u * u * (3 - 2 * u);
        auto vv = v * v * (3 - 2 * v);
        auto ww = w * w * (3 - 2 * w);

        // interpolate
        auto accum = 0.0;
        for (int i = 0; i < 2; i++)
            for (int j = 0; j < 2; j++)
                for (int k = 0; k < 2; k++) {
                    Vector3d weight_v(u - i, v - j, w - k);
                    accum += (i * uu + (1 - i) * (1 - uu))
                             * (j * vv + (1 - j) * (1 - vv))
                             * (k * ww + (1 - k) * (1 - ww))
                             * dot(c[i][j][k], weight_v);
                }
        return accum;
    }
};

#endif //PROJECT_PERLIN_H
