//
// Created by meiyixuan on 2021-12-23.
//

#ifndef PROJECT_DIRECTIONALSKYBOX_H
#define PROJECT_DIRECTIONALSKYBOX_H

class DirectionalSkybox : public Skybox {
public:
    DirectionalSkybox() = default;

    DirectionalSkybox(double _sky_intensity, double _sun_intensity, Vector3d _dir2sun)
            : sky_intensity(_sky_intensity), sun_intensity(_sun_intensity),
              dir2sun(normalize(_dir2sun)) {}

    Color get_color(Ray r) const override {
        auto sun = sun_intensity * pow(fmax(dot(normalize(r.direction()), dir2sun), 0),
                                       20) * Color(1, 1, 0.6);
        auto sky = sky_intensity * Color(0.5, 0.7, 1.0);
        return sun + sky;
    }

private:
    double sky_intensity{2};
    double sun_intensity{10};
    Vector3d dir2sun;
};

#endif //PROJECT_DIRECTIONALSKYBOX_H
