//
// Created by meiyixuan on 2023/12/5.
//

#ifndef PROJECT_UTAHTEAPOT_H
#define PROJECT_UTAHTEAPOT_H

HittableList utah_teapot_scene() {
    HittableList objects;

    // ground
    HittableList boxes1;
    auto ground = make_shared<Lambertian>(Color(0.48, 0.83, 0.53));
    const int boxes_per_side = 140;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 3.0;
            auto x0 = -210.0 + i * w;
            auto z0 = -210.0 + j * w;
            auto y0 = -6.0;
            auto x1 = x0 + w;
            auto y1 = y0 + random_double_fixed(2, 5);
            auto z1 = z0 + w;
            boxes1.add(make_shared<Box>(Point(x0, y0, z0), Point(x1, y1, z1), ground));
        }
    }
    objects.add(make_shared<BVHNode>(boxes1, 0, 1));

    // light
    auto light = make_shared<DiffuseLight>(Color(1, 1, 1), 8);
    objects.add(make_shared<XZRectangle>(-5, 5, -5, 5, 15, light));

    // participating media for whole space
    auto boundary = make_shared<Sphere>(Point(0, 0, 0), 5000,
                                        make_shared<Dielectric>(1.5));
    objects.add(make_shared<ConstantMedium>(boundary, .0001, Color(1, 1, 1)));

    // teapot materials
    auto mesh_0_mat = std::make_shared<Dielectric>(1 / 1.33, Vector3d(0.929, 0.702, 0.518));
    auto mesh_1_mat = std::make_shared<Dielectric>(1.03, Vector3d(0.98, 0.98, 1));
    auto mesh_2_mat = std::make_shared<Dielectric>(1.03 / 1.33, Vector3d(0.929, 0.702, 0.518));
    auto mesh_3_mat = std::make_shared<Dielectric>(1.03, Vector3d(0.98, 0.98, 1));

    // objects
    auto mesh_0_name = model_pth() + "utah-teapot/models/Mesh000.obj";
    auto mesh_1_name = model_pth() + "utah-teapot/models/Mesh001.obj";
    auto mesh_2_name = model_pth() + "utah-teapot/models/Mesh002.obj";
    auto mesh_3_name = model_pth() + "utah-teapot/models/Mesh003.obj";
    auto parser_0 = ObjectParser(mesh_0_name, "");
    auto parser_1 = ObjectParser(mesh_1_name, "");
    auto parser_2 = ObjectParser(mesh_2_name, "");
    auto parser_3 = ObjectParser(mesh_3_name, "");
    objects.add(parser_0.parse(0, 0, Vector3d(), mesh_0_mat));
    objects.add(parser_1.parse(0, 0, Vector3d(), mesh_1_mat));
    objects.add(parser_2.parse(0, 0, Vector3d(), mesh_2_mat));
    objects.add(parser_3.parse(0, 0, Vector3d(), mesh_3_mat));

    return objects;
}

SimpleCamera utah_teapot_camera(double aspect_ratio) {
    // basic settings
    Point camera_position(7, 8, 2);
    Point view_point(0, 2, 0);
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

ConstantSkybox utah_teapot_skybox() { return ConstantSkybox({0, 0, 0}); }

#endif //PROJECT_UTAHTEAPOT_H
