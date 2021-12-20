//
// Created by meiyixuan on 2021-12-09.
// This is dev branch.
//
#include "Headers.h"

void render_scene(int current_id, int max_processes, const char *output_file) {
    // Image settings
#if defined(WINDOWS)
    const auto aspect_ratio = 1.0; // 16.0 / 9.0 or 1.0
    const int image_width = 400; // 3840, 800
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100; // 1000, 100 (x4 if no global light)
    const int max_depth = 10; // 100, 50
#elif defined(DEBUG)
    const auto aspect_ratio = 1.0; // 16.0 / 9.0 or 1.0
    const int image_width = 800; // 3840, 800
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 1000; // 1000, 100 (x4 if no global light)
    const int max_depth = 10; // 100, 50
#else
    const auto aspect_ratio = 1.0; // 16.0 / 9.0 or 1.0
    const int image_width = 3840;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 1000;
    const int max_depth = 50;
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
    HittableList world = test_scene();
    SimpleCamera cam = test_camera(aspect_ratio);
    auto skybox = test_skybox();
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