//
// Created by meiyixuan on 2021-12-29.
//

#ifndef PROJECT_PHOTONMAPPINGINTEGRATOR_H
#define PROJECT_PHOTONMAPPINGINTEGRATOR_H

class PhotonMappingIntegrator : public Integrator {
public:
    PhotonMappingIntegrator(const Accelerator &scene, const Skybox &skybox, std::shared_ptr<PhotonMap> _map)
            : Integrator(scene, skybox), photon_map(std::move(_map)) {}

    Color cast_ray(const Ray &r, int remaining_bounce) const override {
        Hit hit;
        if (world.hit(r, TMIN, inf, hit)) {
            Ray scattered_ray;
            Color emit_color = hit.mat_ptr->emit(hit.u, hit.v, hit.hit_point);
            if (remaining_bounce >= 0 && hit.mat_ptr->scatter(r, hit, scattered_ray)) {
                auto attenuation = hit.mat_ptr->brdf(r, scattered_ray, hit);
                Color light_color;
                if (hit.scatter_mode == 0) {
                    light_color = photon_map->get_irradiance(hit.hit_point, hit.normal, 0.1, 50);
                } else {
                    light_color = cast_ray(scattered_ray, remaining_bounce - 1);
                }
                return emit_color + light_color * attenuation;
            } else {
                return emit_color;
            }
        } else {
            return skybox.get_color(r);
        }
    }

    void trace_photon(const Ray &ray, int remaining_bounce, Vector3d power) {
        Hit hit;
        if (world.hit(ray, TMIN, inf, hit)) {
            Ray scattered_ray;
            if (remaining_bounce > 0 && hit.mat_ptr->scatter(ray, hit, scattered_ray)) {
                // store photon at diffuse surface
                if (hit.scatter_mode == 0) {
                    photon_map->store(Photon(hit.hit_point, ray.direction(), power));
                }
                // generate a new photon
                Color attenuation = hit.mat_ptr->brdf(ray, scattered_ray, hit);
                trace_photon(scattered_ray, remaining_bounce - 1, power * attenuation);
            }
        }
    }

    void trace_photon_caustic(const Ray &ray, int remaining_bounce, Vector3d power) {
        Hit hit;
        if (world.hit(ray, TMIN, inf, hit)) {
            Ray scattered_ray;
            if (remaining_bounce > 0 && hit.mat_ptr->scatter(ray, hit, scattered_ray)) {
                if (hit.scatter_mode == 1 || hit.scatter_mode == 2) {
                    trace_photon(scattered_ray, remaining_bounce - 1, power);
                } else {
                    if (remaining_bounce == 10) return;
                    photon_map->store(Photon(hit.hit_point, ray.direction(), power));
                }
            }
        }
    }

private:
    std::shared_ptr<PhotonMap> photon_map;
};

#endif //PROJECT_PHOTONMAPPINGINTEGRATOR_H
