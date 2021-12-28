//
// Created by meiyixuan on 2021-12-28.
//

#ifndef PROJECT_TWOLAYERSKYBOX_H
#define PROJECT_TWOLAYERSKYBOX_H

class TwoLayerSkybox : public Skybox {
public:
    TwoLayerSkybox() = default;

    TwoLayerSkybox(std::shared_ptr<Skybox> visual, std::shared_ptr<Skybox> GI)
            : visual_sky(std::move(visual)), GI_sky(std::move(GI)) {}

    Color get_color(Ray r) const override {
        if (r.camera_ray()) {
            return visual_sky->get_color(r);
        } else {
            return GI_sky->get_color(r);
        }
    }

private:
    std::shared_ptr<Skybox> visual_sky;
    std::shared_ptr<Skybox> GI_sky;
};

#endif //PROJECT_TWOLAYERSKYBOX_H
