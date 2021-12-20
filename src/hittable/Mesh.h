#include <utility>

//
// Created by meiyixuan on 2021-12-20.
//

#ifndef PROJECT_MESH_H
#define PROJECT_MESH_H

class Mesh : public Hittable {
public:
    Mesh() = default;

    explicit Mesh(std::shared_ptr<BVHNode> _mesh) : mesh(std::move(_mesh)) {}

    explicit Mesh(HittableList &list, double time0 = 0, double time1 = 1)
            : mesh(std::make_shared<BVHNode>(list, time0, time1)) {}

    explicit Mesh(std::vector<shared_ptr<Hittable>> &triangles, double time0 = 0, double time1 = 1)
            : mesh(std::make_shared<BVHNode>(triangles, 0, triangles.size(), time0, time1)) {}

    bool hit(const Ray &ray, double t_min, double t_max, Hit &hit) const override {
        return mesh->hit(ray, t_min, t_max, hit);
    }

    bool bounding_box(double time0, double time1, AABB &output_box) const override {
        return mesh->bounding_box(time0, time1, output_box);
    }

private:
    std::shared_ptr<BVHNode> mesh;

};

#endif //PROJECT_MESH_H
