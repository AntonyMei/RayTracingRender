//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_BACKGROUND_H
#define PROJECT_BACKGROUND_H

class Background {
public:
    virtual Color get_color(Ray r) const = 0;
};

class SimpleSkybox : public Background {
public:
    Color get_color(Ray r) const override {
        Vector3d unit_direction = normalize(r.direction());
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
    }
};

#endif //PROJECT_BACKGROUND_H
