//
// Created by meiyixuan on 2021-12-15.
//

#ifndef PROJECT_INTEGRATOR_H
#define PROJECT_INTEGRATOR_H

class Integrator {
public:
    Integrator() = delete;

    explicit Integrator(const Accelerator &scene, const Skybox &_skybox)
            : world(scene), skybox(_skybox) {}

    virtual Color cast_ray(const Ray &r, int remaining_bounce) const = 0;

protected:
    const Accelerator &world;
    const Skybox &skybox;
};

#endif //PROJECT_INTEGRATOR_H
