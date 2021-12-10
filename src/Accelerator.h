//
// Created by meiyixuan on 2021-12-10.
// This file contains all accelerating data structures.
//

#ifndef PROJECT_ACCELERATOR_H
#define PROJECT_ACCELERATOR_H

#include "memory"
#include "vector"

using std::shared_ptr;
using std::make_shared;

class Accelerator {
public:
    // remove all objects
    virtual void clear() = 0;

    // add object to data storage
    virtual void add(shared_ptr<Hittable> obj) = 0;

    // check if a hit happens
    virtual bool hit(const Ray &r, double t_min, double t_max, Hit &rec) const = 0;
};

class HittableList : public Accelerator {
public:
    HittableList() = default;

    void clear() override { hittable_list.clear(); }

    void add(shared_ptr<Hittable> obj) override { hittable_list.push_back(obj); }

    bool hit(const Ray &r, double t_min, double t_max, Hit &hit) const override {
        // initialize
        Hit obj_hit;
        bool has_hit = false;
        auto closest_t = t_max;

        // check hit status
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

#endif //PROJECT_ACCELERATOR_H
