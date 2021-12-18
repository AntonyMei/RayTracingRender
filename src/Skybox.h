//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_SKYBOX_H
#define PROJECT_SKYBOX_H

class Skybox {
public:
    virtual Color get_color(Ray r) const = 0;
};

class SimpleSkybox : public Skybox {
public:
    Color get_color(Ray r) const override {
        Vector3d unit_direction = normalize(r.direction());
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
    }
};

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

#endif //PROJECT_SKYBOX_H
