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

class SimpleCamera : public Camera {
public:
    SimpleCamera(double vertical_fov, double aspect_ratio) {
        // set intrinsic
        auto theta = deg2rad(vertical_fov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        // set extrinsic
        origin = Point(0, 0, 0);
        horizontal = Vector3d(viewport_width, 0.0, 0.0);
        vertical = Vector3d(0.0, viewport_height, 0.0);
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3d(0, 0, focal_length);
    }

    Ray get_ray(double u, double v) const override {
        return {origin, lower_left_corner + u * horizontal + v * vertical - origin};
    }
};

#endif //PROJECT_CAMERA_H
