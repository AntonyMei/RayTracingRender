//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_MOTIONBLURCHECKER_H
#define PROJECT_MOTIONBLURCHECKER_H

HittableList motion_blur_checker_scene() {
    HittableList world;

    auto checker = make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1),
                                               Color(0.9, 0.9, 0.9));
    world.add(make_shared<Sphere>(Point(0, -1000, 0), 1000,
                                  make_shared<Lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double_fixed();
            Point center(a + 0.9 * random_double_fixed(), 0.15 + 0.1 * random_double_fixed(),
                         b + 0.9 * random_double_fixed());

            if ((center - Point(4, 0.2, 0)).length() > 0.9) {
                shared_ptr <Material> sphere_material;

                if (choose_mat < 0.6) {
                    // diffuse
                    auto albedo = Color::random_fixed() * Color::random_fixed();
                    sphere_material = make_shared<Lambertian>(albedo);
                    auto center2 = center + Vector3d(0, random_double_fixed(0, .5), 0);
                    world.add(make_shared<MovingSphere>(center, center2, 0.0, 1.0,
                                                        center.y(), sphere_material));
                } else if (choose_mat < 0.9) {
                    // metal
                    auto albedo = Color::random_fixed(0.5, 1);
                    auto fuzz = random_double_fixed(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, center.y(), sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, center.y(), sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point(4, 1, 0), 1.0, material3));

    return world;
}

SimpleCamera motion_blur_checker_camera(double aspect_ratio) {
    // basic settings
    Point camera_position(13, 2, 3);
    Point view_point(0, 0, 0);
    Vector3d camera_up(0, 1, 0);
    // fov
    auto vertical_fov = 20.0;
    // off focus blur
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;
    // motion blur (0.0 - 1.0)
    auto shutter_open = 0.0;
    auto shutter_close = 0.5;

    return {camera_position, view_point, camera_up, vertical_fov, aspect_ratio,
            aperture, dist_to_focus, shutter_open, shutter_close};
}

SimpleSkybox motion_blur_checker_skybox() { return {}; }

#endif //PROJECT_MOTIONBLURCHECKER_H
