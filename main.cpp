//
// Created by meiyixuan on 2021-12-09.
//

// standard libraries
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <ctime>

// multiprocessing / multithreading
#ifdef WINDOWS

#include <omp.h>

#else

#include <unistd.h>
#include <sys/wait.h>

#endif

// external libraries
#include "ext/stb_image/stb_image_header.h"

// basic utilities
#include "src/Utils.h"
#include "src/Vector3d.h"
#include "src/Perlin.h"
#include "src/Ray.h"

// classes
#include "src/Pixel.h"
#include "src/Camera.h"
#include "src/AABB.h"
#include "src/Hittable.h"
#include "src/Accelerator.h"
#include "src/Texture.h"
#include "src/Material.h"
#include "src/Light.h"
#include "src/Skybox.h"
#include "src/Integrator.h"

// scenes with global light
HittableList motion_blur_scene() {
    HittableList world;

//    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
//    world.add(make_shared<Sphere>(Point(0, -1000, 0), 1000, ground_material));
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
                shared_ptr<Material> sphere_material;

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

SimpleCamera motion_blur_camera(double aspect_ratio) {
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

HittableList two_spheres_scene() {
    HittableList objects;

    auto checker = make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1),
                                               Color(0.9, 0.9, 0.9));

    objects.add(make_shared<Sphere>(Point(0, -10, 0), 10,
                                    make_shared<Lambertian>(checker)));
    objects.add(make_shared<Sphere>(Point(0, 10, 0), 10,
                                    make_shared<Lambertian>(checker)));

    return objects;
}

SimpleCamera two_spheres_camera(double aspect_ratio) {
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

SimpleSkybox global_light_skybox() { return {}; }

// scenes w/o global light
HittableList simple_light_scene() {
    HittableList objects;

    // objects
    auto marble_texture = make_shared<MarbleTexture>(4);
    objects.add(make_shared<Sphere>(Point(0, -1000, 0), 1000,
                                    make_shared<Lambertian>(marble_texture)));
    objects.add(make_shared<Sphere>(Point(0, 2, 0), 2,
                                    make_shared<Lambertian>(marble_texture)));

    // light
    auto light_mat = make_shared<DiffuseLight>(Color(1, 1, 1), 4);
    objects.add(make_shared<XYRectangle>(3, 5, 1, 3, -2, light_mat));

    return objects;
}

SimpleCamera simple_light_camera(double aspect_ratio) {
    // basic settings
    Point camera_position(26, 3, 6);
    Point view_point(0, 2, 0);
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

HittableList empty_cornell_box_scene() {
    HittableList objects;

    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(1, 1, 1), 15);

    objects.add(make_shared<YZRectangle>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<YZRectangle>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<XZRectangle>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<XZRectangle>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<XZRectangle>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<XYRectangle>(0, 555, 0, 555, 555, white));

    return objects;
}

SimpleCamera empty_cornell_box_camera(double aspect_ratio) {
    // basic settings
    aspect_ratio = 1.0;
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

ConstantSkybox no_global_light_skybox() { return ConstantSkybox({0, 0, 0}); }


void render_scene(int current_id, int max_processes, const char *output_file) {
    // Image settings
#if defined(WINDOWS)
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400; // 3840, 800
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 400; // 1000, 100 (x4 if no global light)
    const int max_depth = 25; // 100, 50
#else
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 3840;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 1000;
    const int max_depth = 100;
#endif

    // image
    auto image = std::vector<std::vector<Pixel>>();
    for (int idx = 0; idx < image_height; ++idx) {
        auto row = std::vector<Pixel>(image_width);
        image.push_back(std::move(row));
    }

    // World, camera and skybox
    // 1. Note that motion blur objects should be created with 0.0 - 1.0.
    //    Control motion blur with camera's shutter.
    // 2. use global_light_skybox if no other lights enabled
    HittableList world = empty_cornell_box_scene();
    SimpleCamera cam = empty_cornell_box_camera(aspect_ratio);
    auto skybox = no_global_light_skybox();
    BVHNode world_bvh(world, cam.shutter_open(), cam.shutter_close());

    // multiprocessing related (id = 0 - max_processes - 1)
    int work_load = image_height / max_processes;
    int start_row = image_height - 1 - work_load * current_id;
    int end_row = current_id == max_processes - 1 ? -1 : start_row - work_load;

    // Render
    PathTracingIntegrator integrator(world_bvh, skybox);
    for (int j = start_row; j > end_row; --j) {
        std::cerr << "Scanlines remaining: " << j - end_row << '\n' << std::flush;
        auto start = time(nullptr);
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color += integrator.cast_ray(r, max_depth);
            }
            image[j][i].set(pixel_color, samples_per_pixel);
        }
        auto end = time(nullptr);
        std::cerr << "loop time " << end - start << "s\n" << std::flush;
    }

    // output
    std::ofstream file;
    file.open(output_file, std::ios::out | std::ios::trunc);
    if (current_id == 0)
        file << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = start_row; j > end_row; --j) {
        for (int i = 0; i < image_width; ++i) {
            image[j][i].write(file);
        }
    }
    file.close();
    std::cerr << "\nDone.\n";
}

int main(int argc, char *argv[]) {
    // parse arguments
    if (argc != 2) {
        std::cerr << "Need to specify process count" << std::endl;
        return 0;
    }
    int max_processes = parse_int(argv[1], strlen(argv[1]));
    std::cerr << "Render will use " << max_processes << " processes." << std::endl;

    // launch real renderer
#if defined(WINDOWS)

    if (max_processes == 1) {
        render_scene(0, 1, "1.ppm");
    } else {
        omp_set_num_threads(max_processes);
#pragma omp parallel for
        for (int thread_idx = 0; thread_idx < max_processes; ++thread_idx) {
            std::string file_name = std::to_string(thread_idx) + ".partial";
            render_scene(thread_idx, max_processes, file_name.c_str());
        }
    }

#else

    if (max_processes == 1) {
        render_scene(0, 1, "1.ppm");
    } else {
        std::vector<pid_t> pid_list;
        for (int process_idx = 0; process_idx < max_processes; ++process_idx) {
            std::string file_name = std::to_string(process_idx) + ".partial";
            pid_t pid = fork();
            if (pid != 0) {
                // parent
                pid_list.push_back(pid);
                continue;
            } else {
                // child
                std::cerr << "Worker " << process_idx << " initialized on " << pid << std::endl;
                render_scene(process_idx, max_processes, file_name.c_str());
                return 0;
            }
        }
        for (auto pid: pid_list) {
            waitpid(pid, NULL, 0);
        }
    }

#endif
}