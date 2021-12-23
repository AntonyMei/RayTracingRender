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
    // 5 10 5 -> 0 0 0
    // 2 1 -2 -> -5 2 1
    Point camera_position(-5, 1, 2);
    Point view_point(5, 3, -1);
    Vector3d camera_up(0, 1, 0);
    // fov
    auto vertical_fov = 70.0;
    // off focus blur
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    // motion blur (0.0 - 1.0)
    auto shutter_open = 0.0;
    auto shutter_close = 1.0;

    return {camera_position, view_point, camera_up, vertical_fov, aspect_ratio,
            aperture, dist_to_focus, shutter_open, shutter_close};
}

//ConstantSkybox sponza_skybox() { return ConstantSkybox(Color(1, 1, 1) * 5); }
//SimpleSkybox sponza_skybox() { return SimpleSkybox(5); }
DirectionalSkybox sponza_skybox() {
    // 1, 5 for sponza
    return {1, 2, normalize(Vector3d(10, 3, 8)), 0.8};
}

#endif //PROJECT_SPONZA_H
