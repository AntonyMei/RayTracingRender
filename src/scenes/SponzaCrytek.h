//
// Created by meiyixuan on 2021-12-26.
//

#ifndef PROJECT_SPONZACRYTEK_H
#define PROJECT_SPONZACRYTEK_H

HittableList sponza_crytek_scene() {
    HittableList objects;

    // mesh
    auto obj_name = model_pth() + "sponza_crytek/sponza.obj";
    auto mtl_path = model_pth() + "sponza_crytek/";
    auto parser = ObjectParser(obj_name, mtl_path);
    objects.add(parser.parse(2, 0,
                             normalize(Vector3d(1, 20, 2))));

    return objects;
}

SimpleCamera sponza_crytek_camera(double aspect_ratio) {
    // basic settings
//    Point camera_position(-5, 1, 2);
//    Point view_point(5, 3, -1);
    Point camera_position(-350, 80, 150);
    Point view_point(0, 170, 0);
    Vector3d camera_up(0, 1, 0);
    // fov
    auto vertical_fov = 75.0;
    // off focus blur
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    // motion blur (0.0 - 1.0)
    auto shutter_open = 0.0;
    auto shutter_close = 1.0;

    return {camera_position, view_point, camera_up, vertical_fov, aspect_ratio,
            aperture, dist_to_focus, shutter_open, shutter_close};
}

DirectionalSkybox sponza_crytek_skybox() {
    // 1, 5 for sponza
    return {1, 12, normalize(Vector3d(1, 20, 2)), 0.8};
}

#endif //PROJECT_SPONZACRYTEK_H
