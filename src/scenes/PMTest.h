//
// Created by meiyixuan on 2021-12-29.
//

#ifndef PROJECT_PMTEST_H
#define PROJECT_PMTEST_H

HittableList PM_test_scene() {
    HittableList objects;

    // wall materials
    auto red = std::make_shared<Lambertian>(std::make_shared<ColorTexture>(0.45, 0.05, 0.05));
    auto white = std::make_shared<Lambertian>(std::make_shared<ColorTexture>(0.53, 0.53, 0.53));
    auto green = std::make_shared<Lambertian>(std::make_shared<ColorTexture>(0.12, 0.45, 0.15));
    // auto bunny_mat = std::make_shared<Dielectric>(1.5, Vector3d(0.87, 0.49, 0.173));
    auto bunny_mat = std::make_shared<Dielectric>(1.5, Vector3d(1, 1, 1));

    // wall objects
    auto l_wall = std::make_shared<YZRectangle>(0, 5.55, 0, 5.55, 5.55, red);
    auto r_wall = std::make_shared<YZRectangle>(0, 5.55, 0, 5.55, 0, green);
    auto ceiling = std::make_shared<XZRectangle>(0, 5.55, 0, 5.55, 5.55, white);
    auto floor = std::make_shared<XZRectangle>(0, 5.55, 0, 5.55, 0, white);
    auto front = std::make_shared<XYRectangle>(0, 5.55, 0, 5.55, 5.55, white);
    auto box = std::make_shared<Box>(Vector3d(0, 0, 0), Vector3d(0.6, 0.6, 0.6), white);
    objects.add(l_wall);
    objects.add(r_wall);
    objects.add(ceiling);
    objects.add(floor);
    objects.add(front);
    objects.add(std::make_shared<Translate>(std::make_shared<RotateY>(box, -18),
                                            Vector3d(3.90, 0, 1.85)));

    // bunny
    auto obj_name = model_pth() + "bunny/bunny.obj";
    auto parser = ObjectParser(obj_name, "");
    objects.add(std::make_shared<Translate>(parser.parse(0, 0,
                                                         Vector3d(), bunny_mat),
                                            Vector3d(1.8, 0.2, 2)));

    return objects;
}

std::shared_ptr<YZRectangleLight> PM_test_light() {
    // light
    auto light_mat = std::make_shared<DiffuseLight>(std::make_shared<ColorTexture>(1, 1, 1),
                                                    27);
    auto light = std::make_shared<YZRectangleLight>(1.13, 2.43, 2.27, 3.32, 0, light_mat);
    return light;
}

SimpleCamera PM_test_camera(double aspect_ratio) {
    // basic settings
    Point camera_position(2.775, 2.775, -3);
    Point view_point(2.775, 2.775, 1);
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

ConstantSkybox PM_test_skybox() { return {}; }

#endif //PROJECT_PMTEST_H
