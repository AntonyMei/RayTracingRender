//
// Created by meiyixuan on 2021-12-10.
// This file contains all accelerating data structures.
//

#ifndef PROJECT_ACCELERATOR_H
#define PROJECT_ACCELERATOR_H

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class Accelerator {
public:
    // remove all objects
    virtual void clear() = 0;

    // add object to data storage
    virtual void add(shared_ptr<Hittable> obj) = 0;

    // check if an intersection happens
    virtual bool intersect(const Ray &r, double t_min, double t_max, Hit &rec) const = 0;
};

// a simple list
class HittableList : public Accelerator {
public:
    HittableList() = default;

    void clear() override { hittable_list.clear(); }

    void add(shared_ptr<Hittable> obj) override { hittable_list.push_back(obj); }

    bool intersect(const Ray &r, double t_min, double t_max, Hit &hit) const override {
        // initialize
        Hit obj_hit;
        bool has_hit = false;
        auto closest_t = t_max;

        // check intersect status
        for (const auto &hittable: hittable_list) {
            if (hittable->hit(r, t_min, closest_t, obj_hit)) {
                has_hit = true;
                closest_t = obj_hit.t;
                hit = obj_hit;
            }
        }

        return has_hit;
    }

private:
    std::vector<std::shared_ptr<Hittable>> hittable_list;
};

// bvh
class AABB {
public:
    AABB() = delete;

    AABB(const Point &min, const Point &max) : minimum(min), maximum(max) {}

    Point min() const { return minimum; }

    Point max() const { return maximum; }

    inline bool hit(const Ray& ray, double t_min, double t_max) const {
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

#endif //PROJECT_ACCELERATOR_H
