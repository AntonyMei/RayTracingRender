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

void render_worker(const int image_width, const int image_height, const int samples_per_pixel, const int max_depth,
                   std::vector<std::vector<Pixel>> &image, const HittableList &world, const SimpleCamera &cam,
                   const int thread_id, const int max_threads) {
    for (int j = image_height - 1; j >= 0; --j) {
        if (j % max_threads != thread_id) continue;
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
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
    }
}

HittableList random_scene() {
    HittableList world;

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point center(a + 0.9 * random_double(), 0.15 + 0.1 * random_double(),
                         b + 0.9 * random_double());

            if ((center - Point(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.75) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, center.y(), sphere_material));
                } else if (choose_mat < 0.9) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
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

void render_hollow_glass_ball() {

    // threading
#if defined(WINDOWS)
    const int thread_count = 16;
    std::clog << "Platform: Windows" << std::endl;
    std::clog << "Thread count: " << thread_count << std::endl;
#else
    const int thread_count = 96;
    std::clog << "Platform: Linux" << std::endl;
    std::clog << "Thread count: " << thread_count << std::endl;
#endif

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
    Point camera_position(0, 0, 0);
    Point view_point(0, 0, -1);
    Vector3d camera_up(0, 1, 0);
    auto dist_to_focus = 1;
    auto vertical_fov = 90;
    auto aperture = 0;
    SimpleCamera cam(camera_position, view_point, camera_up, vertical_fov, aspect_ratio,
                     aperture, dist_to_focus);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    std::vector<std::thread> worker_list;
    for (int thread_id = 0; thread_id < thread_count; ++thread_id) {
        std::thread worker(render_worker, image_width, image_height, samples_per_pixel,
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

void render_hollow_glass_ball_small_fov() {

    // threading
#if defined(WINDOWS)
    const int thread_count = 16;
    std::clog << "Platform: Windows" << std::endl;
    std::clog << "Thread count: " << thread_count << std::endl;
#else
    const int thread_count = 96;
    std::clog << "Platform: Linux" << std::endl;
    std::clog << "Thread count: " << thread_count << std::endl;
#endif

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
    Point camera_position(-2, 2, 1);
    Point view_point(0, 0, -1);
    Vector3d camera_up(0, 1, 0);
    auto dist_to_focus = 1;
    auto vertical_fov = 20;
    auto aperture = 0;
    SimpleCamera cam(camera_position, view_point, camera_up, vertical_fov, aspect_ratio,
                     aperture, dist_to_focus);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    std::vector<std::thread> worker_list;
    for (int thread_id = 0; thread_id < thread_count; ++thread_id) {
        std::thread worker(render_worker, image_width, image_height, samples_per_pixel,
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

void render_hollow_glass_ball_off_focus() {

    // threading
#if defined(WINDOWS)
    const int thread_count = 16;
    std::clog << "Platform: Windows" << std::endl;
    std::clog << "Thread count: " << thread_count << std::endl;
#else
    const int thread_count = 96;
    std::clog << "Platform: Linux" << std::endl;
    std::clog << "Thread count: " << thread_count << std::endl;
#endif

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

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    std::vector<std::thread> worker_list;
    for (int thread_id = 0; thread_id < thread_count; ++thread_id) {
        std::thread worker(render_worker, image_width, image_height, samples_per_pixel,
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
};

void render_many_balls() {

    // threading
#if defined(WINDOWS)
    const int thread_count = 16;
    std::clog << "Platform: Windows" << std::endl;
    std::clog << "Thread count: " << thread_count << std::endl;
#else
    const int thread_count = 96;
    std::clog << "Platform: Linux" << std::endl;
    std::clog << "Thread count: " << thread_count << std::endl;
#endif

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
    auto world = random_scene();

    // Camera
    Point camera_position(13, 2, 3);
    Point view_point(0, 0, 0);
    Vector3d camera_up(0, 1, 0);
    auto dist_to_focus = 10;
    auto vertical_fov = 20;
    auto aperture = 0.1;
    SimpleCamera cam(camera_position, view_point, camera_up, vertical_fov, aspect_ratio,
                     aperture, dist_to_focus);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    std::vector<std::thread> worker_list;
    for (int thread_id = 0; thread_id < thread_count; ++thread_id) {
        std::thread worker(render_worker, image_width, image_height, samples_per_pixel,
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

int main() {
    // multi-tread ray tracer 1.0
    // Note that console output will be scrambled, which is normal.
    render_many_balls();
}