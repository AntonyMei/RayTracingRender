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
        return {};
    }

    void trace_photon(const Ray &ray, int remaining_bounce, Vector3d power) {
        Hit hit;
        if (world.hit(ray, TMIN, inf, hit)) {
            Ray scattered_ray;
            if (remaining_bounce > 0 && hit.mat_ptr->scatter(ray, hit, scattered_ray)) {
                if (hit.scatter_mode == 1) {
                    trace_photon(scattered_ray, remaining_bounce - 1, power);
                } else {
                    photon_map->store(std::make_shared<Photon>(hit.hit_point, ray.direction(), power));
                }
            }
        }
    }

private:
    std::shared_ptr<PhotonMap> photon_map;
};

#endif //PROJECT_PHOTONMAPPINGINTEGRATOR_H
