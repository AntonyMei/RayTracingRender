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
#include "src/Hittable.h"
#include "src/Accelerator.h"
#include "src/Material.h"

Color cast_ray(const Ray &r, const Accelerator &world, int remaining_bounce) {
    Hit hit;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (remaining_bounce <= 0)
        return {0, 0, 0};

    // check intersection
    if (world.intersect(r, TMIN, inf, hit)) {
        std::vector<Ray> scattered_list;
        Color attenuation;
        if (hit.mat_ptr->scatter(r, hit, attenuation, scattered_list)) {
            Color scatter_color;
            for (const auto &scattered: scattered_list) {
                scatter_color += cast_ray(scattered, world, remaining_bounce - 1);
            }
            scatter_color /= static_cast<double>(scattered_list.size());
            return attenuation * scatter_color;
        }
        return {0, 0, 0};
    }

    Vector3d unit_direction = normalize(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

HittableList random_scene() {
    HittableList world;

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point(0, -1000, 0), 1000, ground_material));

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
                    world.add(make_shared<Sphere>(center, center.y(), sphere_material));
                } else if (choose_mat < 0.9) {
                    // metal
                    auto albedo = Color::random_fixed(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
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

void render_scene(int current_id, int max_processes, const char *output_file) {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 3840; // 3840, 800
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 1000; // 1000, 100
    const int max_depth = 100; // 100, 50
    auto image = std::vector<std::vector<Pixel>>();
    for (int idx = 0; idx < image_height; ++idx) {
        auto row = std::vector<Pixel>(image_width);
        image.push_back(std::move(row));
    }

    // World
    HittableList world;
    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left = make_shared<Dielectric>(1.5);
    auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<Sphere>(Point(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Point(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<Sphere>(Point(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(Point(-1.0, 0.0, -1.0), -0.45, material_left));
    world.add(make_shared<Sphere>(Point(1.0, 0.0, -1.0), 0.5, material_right));

    // Camera
    Point camera_position(3, 3, 2);
    Point view_point(0, 0, -1);
    Vector3d camera_up(0, 1, 0);
    auto dist_to_focus = (camera_position - view_point).length();
    auto vertical_fov = 20;
    auto aperture = 1.0;
    SimpleCamera cam(camera_position, view_point, camera_up, vertical_fov, aspect_ratio,
                     aperture, dist_to_focus);

    // multiprocessing related (id = 0 - max_processes - 1)
    int work_load = image_height / max_processes;
    int start_row = image_height - 1 - work_load * current_id;
    int end_row = current_id == max_processes - 1 ? -1 : start_row - work_load;

    // Render
    for (int j = start_row; j > end_row; --j) {
        std::cerr << "Scanlines remaining: " << j - end_row << '\n' << std::flush;
        auto start = time(nullptr);
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color += cast_ray(r, world, max_depth);
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