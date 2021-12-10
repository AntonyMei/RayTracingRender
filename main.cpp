//
// Created by meiyixuan on 2021-12-09.
//
#include <iostream>
#include <vector>
#include "src/Vector3d.h"
#include "src/Pixel.h"
#include "src/Ray.h"
#include "src/Hittable.h"
#include "src/Aggregate.h"

double hit_sphere(const Point& center, double radius, const Ray& r) {
    Vector3d oc = r.origin() - center;
    auto a = r.direction().squared_length();
    auto half_b = dot(oc, r.direction());
    auto c = oc.squared_length() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant) ) / a;
    }
}

Color ray_color(const Ray &r) {
    auto t = hit_sphere(Point(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        Vector3d N = normalize(r.at(t) - Vector3d(0, 0, -1));
        return 0.5 * Color(N.x() + 1, N.y() + 1, N.z() + 1);
    }
    Vector3d unit_direction = normalize(r.direction());
    t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 600;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    auto image = std::vector<std::vector<Pixel>>();
    for (int idx = 0; idx < image_height; ++idx) {
        auto row = std::vector<Pixel>(image_width);
        image.push_back(std::move(row));
    }

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = Point(0, 0, 0);
    auto horizontal = Vector3d(viewport_width, 0, 0);
    auto vertical = Vector3d(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vector3d(0, 0, focal_length);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto u = double(i) / (image_width - 1);
            auto v = double(j) / (image_height - 1);
            Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            Color pixel_color = ray_color(r);
            image[j][i].set(pixel_color);
            image[j][i].write();
        }
    }
    std::cerr << "\nDone.\n";
}