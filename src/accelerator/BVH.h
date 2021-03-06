//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_BVH_H
#define PROJECT_BVH_H

class BVHNode : public Accelerator {
public:
    BVHNode() = default;

    BVHNode(HittableList &list, double time0, double time1)
            : BVHNode(list.hittable_list, 0, list.hittable_list.size(), time0, time1) {}

    BVHNode(std::vector<shared_ptr<Hittable>> &src_objects, size_t start, size_t end,
            double time0, double time1) {
        // surface-area-heuristic BVH build
        // start inclusive, end exclusive

        // special case
        if (src_objects.size() == 1) {
            left = src_objects[0];
            right = nullptr;
            src_objects[0]->bounding_box(time0, time1, box);
            return;
        }

        // initialize
        size_t n = end - start;
        std::vector<AABB> boxes = std::vector<AABB>(n);
        std::vector<double> left_area = std::vector<double>(n);
        std::vector<double> right_area = std::vector<double>(n);
        AABB main_box;
        src_objects[start]->bounding_box(time0, time1, main_box);
        for (int i = 1; i < n; ++i) {
            AABB new_box;
            src_objects[start + i]->bounding_box(time0, time1, new_box);
            main_box = surrounding_box(new_box, main_box);
        }

        // choose split axis
        int axis = main_box.longest_axis();
        if (axis == 0)
            sort(src_objects.begin() + static_cast<long long>(start),
                 src_objects.begin() + static_cast<long long>(end),
                 box_x_compare);
        else if (axis == 1)
            sort(src_objects.begin() + static_cast<long long>(start),
                 src_objects.begin() + static_cast<long long>(end),
                 box_y_compare);
        else
            sort(src_objects.begin() + static_cast<long long>(start),
                 src_objects.begin() + static_cast<long long>(end),
                 box_z_compare);
        for (int i = 0; i < n; ++i)
            src_objects[start + i]->bounding_box(time0, time1, boxes[i]);

        // left area size
        left_area[0] = boxes[0].area();
        AABB left_box = boxes[0];
        for (size_t i = 1; i < n - 1; ++i) {
            left_box = surrounding_box(left_box, boxes[i]);
            left_area[i] = left_box.area();
        }

        // right area size
        right_area[n - 1] = boxes[n - 1].area();
        AABB right_box = boxes[n - 1];
        for (size_t i = n - 2; i > 0; --i) {
            right_box = surrounding_box(right_box, boxes[i]);
            right_area[i] = right_box.area();
        }

        // use SAH heuristic to find split
        double min_SAH = inf;
        size_t min_SAH_idx;
        for (size_t i = 0; i < n - 1; ++i) {
            double SAH = static_cast<double>(i) * left_area[i] +
                         static_cast<double>(n - i - 1) * right_area[i + 1];
            if (SAH < min_SAH) {
                min_SAH_idx = i;
                min_SAH = SAH;
            }
        }

        // split
        if (min_SAH_idx == 0)
            left = src_objects[start];
        else
            left = std::make_shared<BVHNode>(src_objects, start, start + min_SAH_idx + 1,
                                             time0, time1);
        if (min_SAH_idx == n - 2)
            right = src_objects[start + min_SAH_idx + 1];
        else
            right = std::make_shared<BVHNode>(src_objects, start + min_SAH_idx + 1, end,
                                              time0, time1);
        box = main_box;
    }

    bool hit(const Ray &ray, double t_min, double t_max, Hit &hit) const override {
        // if input ray doesn't hit current node
        if (!box.hit(ray, t_min, t_max))
            return false;

        // recursively check two child nodes
        bool hit_left = false, hit_right = false;
        if (left) hit_left = left->hit(ray, t_min, t_max, hit);
        if (right) hit_right = right->hit(ray, t_min, hit_left ? hit.t : t_max, hit);
        return hit_left || hit_right;
    }

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        output_box = box;
        return true;
    }

protected:
    // can be either accelerator or object
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    AABB box;

private:
    static inline bool box_compare(const shared_ptr<Hittable> &a,
                                   const shared_ptr<Hittable> &b, int axis) {
        AABB box_a;
        AABB box_b;

        if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
            std::cerr << "No bounding box found.\n";

        return box_a.min().val[axis] < box_b.min().val[axis];
    }

    static inline bool box_x_compare(const shared_ptr<Hittable> &a, const shared_ptr<Hittable> &b) {
        return box_compare(a, b, 0);
    }

    static inline bool box_y_compare(const shared_ptr<Hittable> &a, const shared_ptr<Hittable> &b) {
        return box_compare(a, b, 1);
    }

    static inline bool box_z_compare(const shared_ptr<Hittable> &a, const shared_ptr<Hittable> &b) {
        return box_compare(a, b, 2);
    }

};

#endif //PROJECT_BVH_H
