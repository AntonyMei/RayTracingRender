//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_SIMPLECAMERA_H
#define PROJECT_SIMPLECAMERA_H

class SimpleCamera : public Camera {
public:
    SimpleCamera(Point cam_pos, Point cam_target, Vector3d cam_up,
                 double vertical_fov, double aspect_ratio,
                 double aperture, double focus_dist,
                 double _time0 = 0, double _time1 = 0) {
        // if no blur: aperture = 0.0, focus_dist = 1.0
        // a nice one: aperture = 0.1, focus_dist = 10
        // if larger blur: aperture = 2.0, focus_dist = dist to focus plane

        // set intrinsic
        auto theta = deg2rad(vertical_fov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        // calculate direction for local coordinates
        w = normalize(cam_pos - cam_target);
        u = normalize(cross(cam_up, w));
        v = cross(w, u);

        // camera extrinsic
        origin = cam_pos;
        horizontal = focus_dist * viewport_width * u;
        vertical = focus_dist * viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;
        lens_radius = aperture / 2;
        time0 = _time0;
        time1 = _time1;
    }

    Ray get_ray(double s, double t) const override {
        // calculate _displacement focus _displacement
        Vector3d rd = lens_radius * random_in_unit_disk();
        Vector3d offset = u * rd.x() + v * rd.y();

        return {origin + offset,
                lower_left_corner + s * horizontal + t * vertical - origin - offset,
                random_double(time0, time1), true};
    }

    double shutter_open() const { return time0; }

    double shutter_close() const { return time1; }

private:
    // three directions of camera
    Vector3d u, v, w;
    // radius = 0: no off-focus blur, 1.0: large blur
    double lens_radius;
    // shutter open/close times
    double time0, time1;
};

#endif //PROJECT_SIMPLECAMERA_H
