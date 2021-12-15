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

    // For BVH
    virtual bool bounding_box(double time0, double time1, AABB &output_box) const = 0;
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

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        // check if there are objects
        if (hittable_list.empty()) return false;

        AABB temp_box;
        bool first_box_flag = true;

        for (const auto &object: hittable_list) {
            // if there are infinitely large object, return false
            if (!object->bounding_box(time0, time1, temp_box)) return false;

            // check if first box
            output_box = first_box_flag ? temp_box : surrounding_box(output_box, temp_box);
            first_box_flag = false;
        }
        return true;
    }

private:
    std::vector<std::shared_ptr<Hittable>> hittable_list;
};

// bvh

#endif //PROJECT_ACCELERATOR_H
