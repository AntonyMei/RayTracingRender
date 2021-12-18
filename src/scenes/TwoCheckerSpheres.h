//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_TWOCHECKERSPHERES_H
#define PROJECT_TWOCHECKERSPHERES_H

HittableList two_checker_spheres_scene() {
    HittableList objects;

    auto checker = make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1),
                                               Color(0.9, 0.9, 0.9));

    objects.add(make_shared<Sphere>(Point(0, -10, 0), 10,
                                    make_shared<Lambertian>(checker)));
    objects.add(make_shared<Sphere>(Point(0, 10, 0), 10,
                                    make_shared<Lambertian>(checker)));

    return objects;
}

SimpleCamera two_checker_spheres_camera(double aspect_ratio) {
    // basic settings
    Point camera_position(13, 2, 3);
    Point view_point(0, 0, 0);
    Vector3d camera_up(0, 1, 0);
    // fov
    auto vertical_fov = 20.0;
    // off focus blur
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    // motion blur (0.0 - 1.0)
    auto shutter_open = 0.0;
    auto shutter_close = 1.0;

    return {camera_position, view_point, camera_up, vertical_fov, aspect_ratio,
            aperture, dist_to_focus, shutter_open, shutter_close};
}

SimpleSkybox two_checker_spheres_skybox() { return {}; }

#endif //PROJECT_TWOCHECKERSPHERES_H
