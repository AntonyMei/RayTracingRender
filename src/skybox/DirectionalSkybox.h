//
// Created by meiyixuan on 2021-12-23.
//

#ifndef PROJECT_DIRECTIONALSKYBOX_H
#define PROJECT_DIRECTIONALSKYBOX_H

class DirectionalSkybox : public Skybox {
public:
    DirectionalSkybox() = default;

    DirectionalSkybox(double _sky_intensity, double _sun_intensity, Vector3d _dir2sun, double _radius)
            : sky_intensity(_sky_intensity), sun_intensity(_sun_intensity),
              dir2sun(normalize(_dir2sun)), sun_radius(_radius) {}

    Color get_color(Ray r) const override {
        if (pow(fmax(dot(normalize(r.direction()), dir2sun), 0), 3) > sun_radius)
            return sun_intensity * Color(1, 1, 1);
        else
            return sky_intensity * Color(0.5, 0.7, 1.0);
    }

private:
    double sky_intensity{1};
    double sun_intensity{3};
    Vector3d dir2sun;
    double sun_radius{0.6};
};

#endif //PROJECT_DIRECTIONALSKYBOX_H
