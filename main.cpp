//
// Created by meiyixuan on 2021-12-09.
//
#include <iostream>
#include <thread>

// basic utilities
#include "src/Utils.h"
#include "src/Vector3d.h"
#include "src/Ray.h"

// classes
#include "src/Pixel.h"
#include "src/Camera.h"
#include "src/Hittable.h"
#include "src/Accelerator.h"

Color ray_color(const Ray &r, const Accelerator &world, int remaining_bounce) {
    Hit hit;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (remaining_bounce <= 0)
        return {0, 0, 0};

    // Note that we only shade front faces
    if (world.intersect(r, 0, inf, hit) && hit.front_face) {
        Point target = hit.hit_point + hit.normal + random_in_unit_sphere();
        return 0.5 * ray_color(Ray(hit.hit_point, target - hit.hit_point), world,
                               remaining_bounce - 1);
    }

    Vector3d unit_direction = normalize(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

void render(const int image_width, const int image_height, const int samples_per_pixel, const int max_depth,
            std::vector<std::vector<Pixel>> &image, HittableList &world, SimpleCamera &cam, const int thread_id,
            const int max_threads) {
    for (int j = image_height - 1; j >= 0; --j) {
        if (j % max_threads != thread_id) continue;
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                Ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            image[j][i].set(pixel_color, samples_per_pixel);
        }
    }
}

int main() {

    // threading
#if defined(WINDOWS)
    const int thread_count = 16;
    std::clog << "Platform: Windows" << std::endl;
    std::clog << "Thread count: " << thread_count << std::endl;
#else
    const int thread_count = 64;
    std::clog << "Platform: Linux" << std::endl;
    std::clog << "Thread count: " << thread_count << std::endl;
#endif

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 800;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;
    auto image = std::vector<std::vector<Pixel>>();
    for (int idx = 0; idx < image_height; ++idx) {
        auto row = std::vector<Pixel>(image_width);
        image.push_back(std::move(row));
    }

    // World
    HittableList world;
    world.add(make_shared<Sphere>(Point(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Point(0, -100.5, -1), 100));

    // Camera
    SimpleCamera cam;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    std::vector<std::thread> worker_list;
    for (int thread_id = 0; thread_id < thread_count; ++thread_id) {
        std::thread worker(render, image_width, image_height, samples_per_pixel,
                           max_depth, std::ref(image), std::ref(world), std::ref(cam),
                           thread_id, thread_count);
        worker_list.push_back(std::move(worker));
    }
    for (auto &worker: worker_list) {
        worker.join();
    }

    // output
    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            image[j][i].write();
        }
    }

    std::cerr << "\nDone.\n";
}
