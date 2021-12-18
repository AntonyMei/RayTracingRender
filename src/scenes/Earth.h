//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_EARTH_H
#define PROJECT_EARTH_H

HittableList earth_scene() {
    HittableList world;
#if defined(WINDOWS)
    auto filename = "earthmap.jpg";
#else
    auto filename = "./resources/earthmap.jpg";
#endif
    auto earth_texture = make_shared<ImageTexture>(filename);
    auto earth_surface = make_shared<Lambertian>(earth_texture);
    auto globe1 = make_shared<Sphere>(Point(0, 0, 0), 2, earth_surface);
    auto globe2 = make_shared<Sphere>(Point(-100, -100, -100), 2, earth_surface);
    world.add(globe1);
    world.add(globe2); // bvh will crash if only one object
    return world;
}

SimpleCamera earth_camera(double aspect_ratio) {
    // basic settings
    Point camera_position(13, 2, 3);
    Point view_point(0, 0, 0);
    Vector3d camera_up(0, 1, 0);
    // fov
    auto vertical_fov = 20.0;
    // off focus blur
    auto dist_to_focus = 10.0;
    auto aperture = 0.0;
    // motion blur (0.0 - 1.0)
    auto shutter_open = 0.0;
    auto shutter_close = 1.0;

    return {camera_position, view_point, camera_up, vertical_fov, aspect_ratio,
            aperture, dist_to_focus, shutter_open, shutter_close};
}

SimpleSkybox earth_skybox() { return {}; }

#endif //PROJECT_EARTH_H
