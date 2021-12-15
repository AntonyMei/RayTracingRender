//
// Created by meiyixuan on 2021-12-15.
//

#ifndef PROJECT_AABB_H
#define PROJECT_AABB_H

class AABB {
public:
    AABB() = default;

    AABB(const Point &min, const Point &max) : minimum(min), maximum(max) {}

    Point min() const { return minimum; }

    Point max() const { return maximum; }

    inline bool hit(const Ray &ray, double t_min, double t_max) const {
        // an optimized slab method (Andrew Kensler at Pixar)
        for (int a = 0; a < 3; a++) {
            auto invD = 1.0f / ray.direction()[a];
            auto t0 = (min()[a] - ray.origin()[a]) * invD;
            auto t1 = (max()[a] - ray.origin()[a]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min)
                return false;
        }
        return true;
    }


private:
    // ranges in three directions (i.e. two points in diagonal)
    Point minimum;
    Point maximum;
};

AABB surrounding_box(AABB box0, AABB box1) {
    Point min(fmin(box0.min().x(), box1.min().x()),
              fmin(box0.min().y(), box1.min().y()),
              fmin(box0.min().z(), box1.min().z()));

    Point max(fmax(box0.max().x(), box1.max().x()),
              fmax(box0.max().y(), box1.max().y()),
              fmax(box0.max().z(), box1.max().z()));

    return {min, max};
}

#endif //PROJECT_AABB_H
