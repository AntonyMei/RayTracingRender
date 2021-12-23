//
// Created by meiyixuan on 2021-12-23.
//

#ifndef PROJECT_TESTTRIANGLE_H
#define PROJECT_TESTTRIANGLE_H

HittableList test_triangle_scene() {
    HittableList objects;

    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(1, 1, 1), 15);
#if defined(WINDOWS)
    auto filename = "earthmap.jpg";
#else
    auto filename = "./resources/earthmap.jpg";
#endif
    auto earth_texture = make_shared<ImageTexture>(filename);
    auto earth_surface = make_shared<Lambertian>(earth_texture);

    // wall
    objects.add(make_shared<YZRectangle>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<YZRectangle>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<XZRectangle>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<XZRectangle>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<XZRectangle>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<XYRectangle>(0, 555, 0, 555, 555, white));

    // triangle
    auto v0 = make_shared<Vertex>(0.0, 0.0, Point(200, 200, 200));
    auto v1 = make_shared<Vertex>(0.0, 1.0, Point(200, 300, 300));
    auto v2 = make_shared<Vertex>(1.0, 0.0, Point(400, 200, 200));
    objects.add(std::make_shared<Triangle>(v0, v1, v2, earth_surface));

    return objects;
}

SimpleCamera test_triangle_camera(double aspect_ratio) {
    // basic settings
    Point camera_position(278, 278, -800);
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

ConstantSkybox test_triangle_skybox() { return ConstantSkybox({0, 0, 0}); }

#endif //PROJECT_TESTTRIANGLE_H
