//
// Created by meiyixuan on 2021-12-20.
//

#ifndef PROJECT_TEST_H
#define PROJECT_TEST_H

HittableList test_obj_scene() {
    HittableList objects;

    // mesh
    auto obj_name = model_pth() + "CornellBox-Original.obj";
    auto mtl_path = model_pth();
    objects.add(ObjectParser(obj_name, mtl_path).parse());

    return objects;
}

SimpleCamera test_obj_camera(double aspect_ratio) {
    // basic settings
    Point camera_position(0, 1, 0);
    Point view_point(-1.2, 1, 0);
    Vector3d camera_up(0, 1, 0);
    // fov
    auto vertical_fov = 40.0;
    // off focus blur
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    // motion blur (0.0 - 1.0)
    auto shutter_open = 0.0;
    auto shutter_close = 1.0;

    return {camera_position, view_point, camera_up, vertical_fov, aspect_ratio,
            aperture, dist_to_focus, shutter_open, shutter_close};
}

ConstantSkybox test_obj_skybox() { return ConstantSkybox({1, 1, 1}); }

#endif //PROJECT_TEST_H
