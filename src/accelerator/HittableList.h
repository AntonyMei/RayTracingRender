//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_HITTABLELIST_H
#define PROJECT_HITTABLELIST_H

class HittableList : public Accelerator {
public:
    HittableList() = default;

    void clear() { hittable_list.clear(); }

    void add(const shared_ptr<Hittable> &obj) { hittable_list.push_back(obj); }

    bool hit(const Ray &r, double t_min, double t_max, Hit &hit) const override {
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

protected:
    friend class BVHNode;

    std::vector<std::shared_ptr<Hittable>> hittable_list;
};

#endif //PROJECT_HITTABLELIST_H
