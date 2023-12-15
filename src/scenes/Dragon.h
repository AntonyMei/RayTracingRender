//
// Created by meiyixuan on 2023/12/8.
//

#ifndef PROJECT_DRAGON_H
#define PROJECT_DRAGON_H

HittableList dragon_scene() {
    HittableList objects;

    // ground
    HittableList floor;
    auto texture_filename = "./resources/dragon/ground_texture.jpg";
    auto ground_mat = make_shared<Lambertian>(make_shared<ImageTexture>(texture_filename, 0));
    const int boxes_per_side = 50;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 60.0;
            auto x0 = -1500.0 + i * w;
            auto z0 = -1500.0 + j * w;
            auto y0 = -80.0;
            auto x1 = x0 + w;
            auto y1 = y0 + 1;
            auto z1 = z0 + w;
            floor.add(make_shared<Box>(Point(x0, y0, z0), Point(x1, y1, z1), ground_mat));
        }
    }
    objects.add(make_shared<BVHNode>(floor, 0, 1));

    // stand
    auto stand = make_shared<Lambertian>(make_shared<ImageTexture>(texture_filename, 0));
    objects.add(make_shared<Box>(Vector3d(-20, -80, -25), Vector3d(20, -2, 15), stand));

    // light frame
    objects.add(make_shared<Box>(Vector3d(-20, 40.1, -25), Vector3d(20, 200, 15), stand));
    objects.add(make_shared<Box>(Vector3d(-20, 40, -25), Vector3d(20, 35, -22), stand));
    objects.add(make_shared<Box>(Vector3d(20, 40, -25), Vector3d(17, 35, 15), stand));
    objects.add(make_shared<Box>(Vector3d(-20, 40, 15), Vector3d(-17, 35, 15), stand));
    objects.add(make_shared<Box>(Vector3d(20, 40, 15), Vector3d(-20, 35, 12), stand));

    // participating media for whole space
    auto boundary = make_shared<Sphere>(Point(0, 0, 0), 5000,
                                        make_shared<Dielectric>(1.5));
    objects.add(make_shared<ConstantMedium>(boundary, .0001, Color(1, 1, 1)));

    // light 1
    auto light = make_shared<DiffuseLight>(Color(1, 1, 1), 5);
    objects.add(make_shared<XZRectangle>(-20, 20, -25, 15, 40, light));

    // dragon material
    auto dragon_mat = std::make_shared<Lambertian>(Vector3d(0.79311, 0.79311, 0.79311));

    // add the dragon
    for (int i = 0; i < 16; ++i) {
        std::stringstream _idx;
        _idx << std::setw(3) << std::setfill('0') << i;
        auto mesh_name = model_pth() + "dragon/models/Mesh" + _idx.str() + ".obj";
        auto parser = ObjectParser(mesh_name, "");
        objects.add(parser.parse(0, 0, Vector3d(), dragon_mat));
    }

    return objects;
}

SimpleCamera dragon_camera(double aspect_ratio) {
    // Special notice: for this figure, the aspect ratio should be 9.0 / 16.0
    // basic settings
    Point camera_position(80, 20, 60);
    Point view_point(0, 18, -5);
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

ConstantSkybox dragon_skybox() { return ConstantSkybox({0, 0, 0}); }
//DirectionalSkybox dragon_skybox() {
//    return {1, 5, normalize(Vector3d(1, 20, 2)), 0.8};
//}

#endif //PROJECT_DRAGON_H
