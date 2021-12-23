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

//    auto mat_ptr = std::make_shared<PBRMaterial>("", "",
//                                                 Color(0, 0, 0), "",
//                                                 Color(0.8, 0.6, 0.2), 1, "",
//                                                 Color(0, 0, 0), "",
//                                                 1.5, Color(0.2, 0.2, 0.2), 1);
//    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
//    auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
//    auto material_left = make_shared<Dielectric>(1.5);
//    auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);
//
//    objects.add(make_shared<Sphere>(Point(0.0, -100.5, -1.0), 100.0, material_ground));
//    objects.add(make_shared<Sphere>(Point(0.0, 0.0, -1.0), 0.5, material_center));
//    objects.add(make_shared<Sphere>(Point(-1.0, 0.0, -1.0), 0.5, material_left));
//    objects.add(make_shared<Sphere>(Point(-1.0, 0.0, -1.0), -0.45, material_left));
//    objects.add(make_shared<Sphere>(Point(1.0, 0.0, -1.0), 0.5, mat_ptr));

    return objects;
}

SimpleCamera sponza_camera(double aspect_ratio) {
    // basic settings
    // 5 10 5 -> 0 0 0
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

//    // test brdf
//    Point camera_position(0, 0, 0);
//    Point view_point(0, 0, -1);
//    Vector3d camera_up(0, 1, 0);
//    auto dist_to_focus = 1.0;
//    auto vertical_fov = 90.0;
//    auto aperture = 0.0;

    return {camera_position, view_point, camera_up, vertical_fov, aspect_ratio,
            aperture, dist_to_focus, shutter_open, shutter_close};
}

//ConstantSkybox sponza_skybox() { return ConstantSkybox(Color(1, 1, 1) * 5); }
SimpleSkybox sponza_skybox() { return SimpleSkybox(5); }

#endif //PROJECT_SPONZA_H
