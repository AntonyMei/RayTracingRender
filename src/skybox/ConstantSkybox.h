//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_CONSTANTSKYBOX_H
#define PROJECT_CONSTANTSKYBOX_H

class ConstantSkybox : public Skybox {
public:
    ConstantSkybox() = default;

    explicit ConstantSkybox(Color _sky_color) : sky_color(_sky_color) {}

    Color get_color(Ray r) const override {
        return sky_color;
    }

private:
    Color sky_color;
};

#endif //PROJECT_CONSTANTSKYBOX_H
