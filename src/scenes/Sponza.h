//
// Created by meiyixuan on 2021-12-23.
//

#ifndef PROJECT_SPONZA_H
#define PROJECT_SPONZA_H

HittableList sponza_scene() {
    HittableList objects;

    // mesh
    auto obj_name = model_pth() + "sponza/sponza.obj";
    auto mtl_path = model_pth() + "sponza/";
    objects.add(ObjectParser(obj_name, mtl_path).parse());

    return objects;
}

SimpleCamera sponza_camera(double aspect_ratio) {
    // basic settings
    Point camera_position(5, 10, 5);
    Point view_point(0, 0, 0);
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

//ConstantSkybox sponza_skybox() { return ConstantSkybox({0, 0, 0}); }
SimpleSkybox sponza_skybox() { return SimpleSkybox(10); }

#endif //PROJECT_SPONZA_H
