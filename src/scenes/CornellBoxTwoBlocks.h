//
// Created by meiyixuan on 2021-12-19.
//

#ifndef PROJECT_CORNELLBOXTWOBLOCKS_H
#define PROJECT_CORNELLBOXTWOBLOCKS_H

HittableList cornell_box_two_blocks_scene() {
    HittableList objects;

    // materials
    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(1, 1, 1), 15);

    // cornell box
    objects.add(make_shared<YZRectangle>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<YZRectangle>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<XZRectangle>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<XZRectangle>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<XZRectangle>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<XYRectangle>(0, 555, 0, 555, 555, white));

    // blocks
    objects.add(make_shared<Box>(Point(130, 0, 65), Point(295, 165, 230), white));
    objects.add(make_shared<Box>(Point(265, 0, 295), Point(430, 330, 460), white));

    return objects;
}

SimpleCamera cornell_box_two_blocks_camera(double aspect_ratio) {
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

ConstantSkybox cornell_box_two_blocks_skybox() { return ConstantSkybox({0, 0, 0}); }

#endif //PROJECT_CORNELLBOXTWOBLOCKS_H
