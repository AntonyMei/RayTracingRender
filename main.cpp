//
// Created by meiyixuan on 2021-12-09.
//
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

// basic utilities
#include "src/Utils.h"
#include "src/Vector3d.h"
#include "src/Ray.h"

// classes
#include "src/Pixel.h"
#include "src/Camera.h"
#include "src/AABB.h"
#include "src/Hittable.h"
#include "src/Accelerator.h"
#include "src/Texture.h"
#include "src/Material.h"
#include "src/Integrator.h"

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

void render_scene(int current_id, int max_processes, const char *output_file) {
    // Image settings
#if defined(WINDOWS)
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400; // 3840, 800
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100; // 1000, 100
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

    // World & camera
    // Note that motion blur objects should be created with 0.0 - 1.0
    // Control motion blur with camera's shutter
    HittableList world = two_spheres_scene();
    SimpleCamera cam = two_spheres_camera(aspect_ratio);
    BVHNode world_bvh(world, cam.shutter_open(), cam.shutter_close());

    // multiprocessing related (id = 0 - max_processes - 1)
    int work_load = image_height / max_processes;
    int start_row = image_height - 1 - work_load * current_id;
    int end_row = current_id == max_processes - 1 ? -1 : start_row - work_load;

    // Render
    PathTracingIntegrator integrator(world_bvh);
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