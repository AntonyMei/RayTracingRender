#include <utility>

//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_BOX_H
#define PROJECT_BOX_H

class Box : public Hittable {
public:
    Box() = default;

    Box(const Point &p0, const Point &p1, const std::shared_ptr<Material> &mat_ptr) : Hittable(nullptr) {
        // diagonal points
        min_point = p0;
        max_point = p1;

        // construct faces
        faces.add(make_shared<XYRectangle>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat_ptr));
        faces.add(make_shared<XYRectangle>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat_ptr));
        faces.add(make_shared<XZRectangle>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat_ptr));
        faces.add(make_shared<XZRectangle>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat_ptr));
        faces.add(make_shared<YZRectangle>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat_ptr));
        faces.add(make_shared<YZRectangle>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat_ptr));
    }

    bool hit(const Ray &ray, double t_min, double t_max, Hit &hit) const override {
        return faces.hit(ray, t_min, t_max, hit);
    }

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        output_box = AABB(min_point, max_point);
        return true;
    }

private:
    Point min_point, max_point;
    HittableList faces;
};

#endif //PROJECT_BOX_H
