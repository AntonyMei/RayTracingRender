//
// Created by meiyixuan on 2021-12-19.
//

#ifndef PROJECT_BOOK2FINAL_H
#define PROJECT_BOOK2FINAL_H

HittableList book_2_final_scene() {

    HittableList objects;

    // ground
    HittableList boxes1;
    auto ground = make_shared<Lambertian>(Color(0.48, 0.83, 0.53));
    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double_fixed(1, 101);
            auto z1 = z0 + w;
            boxes1.add(make_shared<Box>(Point(x0, y0, z0), Point(x1, y1, z1), ground));
        }
    }
    objects.add(make_shared<BVHNode>(boxes1, 0, 1));

    // light
    auto light = make_shared<DiffuseLight>(Color(1, 1, 1), 7);
    objects.add(make_shared<XZRectangle>(123, 423, 147, 412, 554, light));

    // moving sphere
    auto center1 = Point(400, 400, 200);
    auto center2 = center1 + Vector3d(30, 0, 0);
    auto moving_sphere_material = make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    objects.add(make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    // dielectric and metal sphere
    objects.add(make_shared<Sphere>(Point(260, 150, 45), 50,
                                    make_shared<Dielectric>(1.5)));
    objects.add(make_shared<Sphere>(Point(0, 150, 145), 50,
                                    make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)
    ));

    // participating media ball
    auto boundary = make_shared<Sphere>(Point(360, 150, 145), 70,
                                        make_shared<Dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));

    // participating media for whole space
    boundary = make_shared<Sphere>(Point(0, 0, 0), 5000,
                                   make_shared<Dielectric>(1.5));
    objects.add(make_shared<ConstantMedium>(boundary, .0001, Color(1, 1, 1)));

    // earth ball
#if defined(WINDOWS)
    auto filename = "earthmap.jpg";
#else
    auto filename = "./resources/earthmap.jpg";
#endif
    auto earth_mat = make_shared<Lambertian>(make_shared<ImageTexture>(filename));
    objects.add(make_shared<Sphere>(Point(400, 200, 400), 100, earth_mat));

    // marble ball
    auto marble_texture = make_shared<MarbleTexture>(4);
    objects.add(make_shared<Sphere>(Point(220, 280, 300), 80,
                                    make_shared<Lambertian>(marble_texture)));

    // box with spheres
    HittableList boxes2;
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<Sphere>(Point::random_fixed(0, 165), 10, white));
    }
    objects.add(make_shared<Translate>(
            make_shared<RotateY>(
                    make_shared<BVHNode>(boxes2, 0.0, 1.0), 30),
            Vector3d(-100, 270, 395)));

    return objects;
}

SimpleCamera book_2_final_camera(double aspect_ratio) {
    // basic settings
    Point camera_position(478, 278, -600);
    Point view_point(278, 278, 0);
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

ConstantSkybox book_2_final_skybox() { return ConstantSkybox({0, 0, 0}); }

#endif //PROJECT_BOOK2FINAL_H
