//
// Created by meiyixuan on 2021-12-23.
//

#ifndef PROJECT_SANMIGUEL_H
#define PROJECT_SANMIGUEL_H

HittableList san_miguel_scene() {
    HittableList objects;

    // mesh
    auto obj_name = model_pth() + "san_miguel/san-miguel.obj";
    auto mtl_path = model_pth() + "san_miguel/";
    auto parser = ObjectParser(obj_name, mtl_path);
    objects.add(parser.parse(0, 0.1, normalize(Vector3d(1, 20, 2))));
    std::cout << "scene loaded!" << std::endl;

    return objects;
}

SimpleCamera san_miguel_camera_1(double aspect_ratio) {
    // under the trees and look right at floor 1
    // use skybox 1 / 2
    Point camera_position(17.7, 1.6, 7.4);
    Point view_point(3, 3, -3);
    Vector3d camera_up(0, 1, 0);
    // fov
    auto vertical_fov = 90.0;
    // off focus blur
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    // motion blur (0.0 - 1.0)
    auto shutter_open = 0.0;
    auto shutter_close = 1.0;

    return {camera_position, view_point, camera_up, vertical_fov, aspect_ratio,
            aperture, dist_to_focus, shutter_open, shutter_close};
}

SimpleCamera san_miguel_camera_2(double aspect_ratio) {
    // floor 2, near entrance
    // use skybox 1 / 2
    Point camera_position(16, 7, -2);
    Point view_point(0, 7, 0);
    Vector3d camera_up(0, 1, 0);
    // fov
    auto vertical_fov = 90.0;
    // off focus blur
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    // motion blur (0.0 - 1.0)
    auto shutter_open = 0.0;
    auto shutter_close = 1.0;

    return {camera_position, view_point, camera_up, vertical_fov, aspect_ratio,
            aperture, dist_to_focus, shutter_open, shutter_close};
}

SimpleCamera san_miguel_camera_3(double aspect_ratio) {
    // floor 1, under roof
    // use skybox 1 / 2
    Point camera_position(16, 1.6, -2);
    Point view_point(0, 3, 6);
    Vector3d camera_up(0, 1, 0);
    // fov
    auto vertical_fov = 90.0;
    // off focus blur
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    // motion blur (0.0 - 1.0)
    auto shutter_open = 0.0;
    auto shutter_close = 1.0;

    return {camera_position, view_point, camera_up, vertical_fov, aspect_ratio,
            aperture, dist_to_focus, shutter_open, shutter_close};
}

SimpleCamera san_miguel_camera_4(double aspect_ratio) {
    // Note: not tested
    // floor 2, far away from entrance
    // use skybox 1 / 2
    Point camera_position(20, 7, -2);
    Point view_point(0, 7, 5);
    Vector3d camera_up(0, 1, 0);
    // fov
    auto vertical_fov = 90.0;
    // off focus blur
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    // motion blur (0.0 - 1.0)
    auto shutter_open = 0.0;
    auto shutter_close = 1.0;

    return {camera_position, view_point, camera_up, vertical_fov, aspect_ratio,
            aperture, dist_to_focus, shutter_open, shutter_close};
}

DirectionalSkybox san_miguel_skybox_1() {
    // 1, 5 for san_miguel
    return {1, 5, normalize(Vector3d(1, 20, 2)), 0.8};
}

DirectionalSkybox san_miguel_skybox_2() {
    // 1, 10 for brighter san_miguel
    // this is the recommended one
    return {1, 10, normalize(Vector3d(1, 20, 2)), 0.8};
}

#endif //PROJECT_SANMIGUEL_H
