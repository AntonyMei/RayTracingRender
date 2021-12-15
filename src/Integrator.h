//
// Created by meiyixuan on 2021-12-15.
//

#ifndef PROJECT_INTEGRATOR_H
#define PROJECT_INTEGRATOR_H

class Integrator {
public:
    Integrator() = delete;

    explicit Integrator(const Accelerator &scene) : world(scene) {}

    virtual Color cast_ray(const Ray &r, int remaining_bounce) const = 0;

protected:
    const Accelerator &world;
};

class PathTracingIntegrator : Integrator {
public:
    PathTracingIntegrator() = delete;

    explicit PathTracingIntegrator(const Accelerator &scene) : Integrator(scene) {}

    Color cast_ray(const Ray &r, int remaining_bounce) const override {
        Hit hit;

        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (remaining_bounce <= 0)
            return {0, 0, 0};

        // check intersection
        if (world.intersect(r, TMIN, inf, hit)) {
            std::vector<Ray> scattered_list;
            Color attenuation;
            if (hit.mat_ptr->scatter(r, hit, attenuation, scattered_list)) {
                Color scatter_color;
                for (const auto &scattered: scattered_list) {
                    scatter_color += cast_ray(scattered, remaining_bounce - 1);
                }
                scatter_color /= static_cast<double>(scattered_list.size());
                return attenuation * scatter_color;
            }
            return {0, 0, 0};
        }

        Vector3d unit_direction = normalize(r.direction());
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
    }
};

#endif //PROJECT_INTEGRATOR_H