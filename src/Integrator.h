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

class PathTracingIntegrator : Integrator {
public:
    PathTracingIntegrator() = delete;

    explicit PathTracingIntegrator(const Accelerator &scene, const Skybox &skybox)
            : Integrator(scene, skybox) {}

    Color cast_ray(const Ray &r, int remaining_bounce) const override {
        Hit hit;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (remaining_bounce <= 0) return {0, 0, 0};

        // If hit nothing, return background
        if (!world.hit(r, TMIN, inf, hit)) return skybox.get_color(r);

        // hit something
        std::vector<Ray> scattered_list;
        Color attenuation;
        Color emit_color = hit.mat_ptr->emit(hit.u, hit.v, hit.hit_point);

        // no scattering
        if (!hit.mat_ptr->scatter(r, hit, attenuation, scattered_list))
            return emit_color;

        // scattering
        Color scatter_color;
        for (const auto &scattered: scattered_list) {
            scatter_color += cast_ray(scattered, remaining_bounce - 1);
        }
        scatter_color /= static_cast<double>(scattered_list.size());
        return emit_color + attenuation * scatter_color;
    }
};

#endif //PROJECT_INTEGRATOR_H
