//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_SIMPLELIGHT_H
#define PROJECT_SIMPLELIGHT_H

HittableList simple_light_scene() {
    HittableList objects;

    // objects
    auto marble_texture = make_shared<MarbleTexture>(4);
    objects.add(make_shared<Sphere>(Point(0, -1000, 0), 1000,
                                    make_shared<Lambertian>(marble_texture)));
    objects.add(make_shared<Sphere>(Point(0, 2, 0), 2,
                                    make_shared<Lambertian>(marble_texture)));

    // light
    auto light_mat = make_shared<DiffuseLight>(Color(1, 1, 1), 4);
    objects.add(make_shared<XYRectangle>(3, 5, 1, 3, -2, light_mat));

    return objects;
}

SimpleCamera simple_light_camera(double aspect_ratio) {
    // basic settings
    Point camera_position(26, 3, 6);
    Point view_point(0, 2, 0);
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

ConstantSkybox simple_light_skybox() { return ConstantSkybox({0, 0, 0}); }

#endif //PROJECT_SIMPLELIGHT_H
