//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_TWOMARBLESPHERES_H
#define PROJECT_TWOMARBLESPHERES_H

HittableList two_marble_spheres_scene() {
    HittableList objects;

    auto perlin_texture = make_shared<MarbleTexture>(4);
    objects.add(make_shared<Sphere>(Point(0, -1000, 0), 1000,
                                    make_shared<Lambertian>(perlin_texture)));
    objects.add(make_shared<Sphere>(Point(0, 2, 0), 2,
                                    make_shared<Lambertian>(perlin_texture)));

    return objects;
}

SimpleCamera two_marble_spheres_camera(double aspect_ratio) {
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

SimpleSkybox two_marble_spheres_skybox() { return {}; }

#endif //PROJECT_TWOMARBLESPHERES_H
