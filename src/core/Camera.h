//
// Created by meiyixuan on 2021-12-11.
//

#ifndef PROJECT_CAMERA_H
#define PROJECT_CAMERA_H

class Camera {
public:
    virtual Ray get_ray(double u, double v) const = 0;

protected:
    // camera extrinsic
    Point origin;
    Point lower_left_corner;
    Vector3d horizontal;
    Vector3d vertical;
};

#endif //PROJECT_CAMERA_H
