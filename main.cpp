//
// Created by meiyixuan on 2021-12-09.
//
#include <iostream>

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

Color ray_color(const Ray &r, const Accelerator &world, int remaining_bounce) {
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
            for (const auto& scattered: scattered_list) {
                scatter_color += ray_color(scattered, world, remaining_bounce - 1);
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

int main() {
    // removed threading
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1024;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 1;
    const int max_depth = 50;
    auto image = std::vector<std::vector<Pixel>>();
    for (int idx = 0; idx < image_height; ++idx) {
        auto row = std::vector<Pixel>(image_width);
        image.push_back(std::move(row));
    }

    // World
    HittableList world;
    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
    auto material_left   = make_shared<Metal>(Color(0.8, 0.8, 0.8));
    auto material_right  = make_shared<Metal>(Color(0.8, 0.6, 0.2));

    world.add(make_shared<Sphere>(Point( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Point( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<Sphere>(Point(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<Sphere>(Point( 1.0,    0.0, -1.0),   0.5, material_right));

    // Camera
    SimpleCamera cam;

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; --j) {
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

    // output
    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            image[j][i].write();
        }
    }

    std::cerr << "\nDone.\n";
}