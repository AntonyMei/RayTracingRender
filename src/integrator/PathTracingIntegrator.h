//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_PATHTRACINGINTEGRATOR_H
#define PROJECT_PATHTRACINGINTEGRATOR_H

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
        Ray scattered_ray;
        Color emit_color = hit.mat_ptr->emit(hit.u, hit.v, hit.hit_point);

        // no scattering
        if (!hit.mat_ptr->scatter(r, hit, scattered_ray))
            return emit_color;

        // scattering
        Color attenuation = hit.mat_ptr->brdf(r, scattered_ray, hit);
        Color scatter_color = cast_ray(scattered_ray, remaining_bounce - 1);
        return emit_color + attenuation * scatter_color;
    }
};

#endif //PROJECT_PATHTRACINGINTEGRATOR_H
