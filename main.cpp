//
// Created by meiyixuan on 2021-12-09.
// This is our final result, RayTracer7.0. Everything has been finished. See you in the next project!
//
/**************************** Usage ****************************/
//
// Note: If compiled on Windows, flag $WINDOWS$ is automatically set. If compiled
//       on Linux, can use $DEBUG$ to switch between debug run (fast) or normal
//       run (high quality).
//
// How set scene before compile:
// step 1: set image settings (customization (e.g. changing aspect_ratio to
//         16.0 / 9.0) can be done by modifying render_scene in main.cpp)
// step 2: choose integrator (Photon Mapping only supports one scene, Path Tracing has step 3
//         of setting scene)
// step 3: set scene, camera and skybox (scene functions can be found in src/scenes)
//
// How to run this renderer:
// Windows:
// step 1: compile with Clion using MSVC compiler
// step 2: run "Project.exe 8", here 8 means using 8 threads
//         (after running we will get 8 .partial files)
// step 3: run "packager.exe 8", here 8 means combining 8 partial files
//         (after running we will get a .ppm image, which can be opened
//         using OpenSeeIt)
// step 4: run python convert.py, which converts .ppm into .jpg
//         (note that opencv for python is required to run this)
// Linux:
// run "bash linux_run.sh", which compiles the renderer, uses 80 processes
// to run it and process result into a .ppm and a .jpg file. (Note that for
// default SponzaCrytek scene, about 150 GB memory is required. This requirement
// is proportional to the number of processes.)
//
/***************************************************************/
#include "Headers.h"

void render_scene(int current_id, int max_processes, const char *output_file) {
    // Image settings
#if defined(WINDOWS)
    const auto aspect_ratio = 1.0; // 16.0 / 9.0 or 1.0
    const int image_width = 400; // 3840, 800
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100; // 1000, 100 (x4 if no global light)
    const int max_depth = 10; // 100, 50
    const int photon_map_size = 500000;
#elif defined(DEBUG)
    const auto aspect_ratio = 1.0; // 16.0 / 9.0 or 1.0
    const int image_width = 800; // 3840, 800
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100; // 1000, 100 (x4 if no global light)
    const int max_depth = 10; // 100, 50
    const int photon_map_size = 5000000;
#else
    const auto aspect_ratio = 1.0; // 16.0 / 9.0 or 1.0
    const int image_width = 3840;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 1000;
    const int max_depth = 50;
    const int photon_map_size = 20000000;
#endif

    // image
    auto image = std::vector<std::vector<Pixel>>();
    for (int idx = 0; idx < image_height; ++idx) {
        auto row = std::vector<Pixel>(image_width);
        image.push_back(std::move(row));
    }

    // multiprocessing related (id = 0 - max_processes - 1)
    int work_load = image_height / max_processes;
    int start_row = image_height - 1 - work_load * current_id;
    int end_row = current_id == max_processes - 1 ? -1 : start_row - work_load;

    // Render
    /************************* Integrator *************************/
    // first specify use_photon_map() or use_path_tracing()
    // integrator_type = use_photon_map();
    integrator_type = use_path_tracing();
    /**************************************************************/
    if (integrator_type == 0) {
        // World, camera and skybox
        // 1. Note that motion blur objects should be created with 0.0 - 1.0.
        //    Control motion blur with camera's shutter.
        // 2. use global_light_skybox if no other lights enabled
        /************************** Scene **************************/
        HittableList world = sponza_crytek_scene();
        SimpleCamera cam = sponza_crytek_camera(aspect_ratio);
        auto skybox = sponza_crytek_skybox_cloudy();
        /**********************************************************/
        BVHNode world_bvh(world, cam.shutter_open(), cam.shutter_close());

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
    } else if (integrator_type == 1) {
        // World, camera and skybox
        HittableList world = PM_test_scene();
        auto light = PM_test_light();
        world.add(light);
        SimpleCamera cam = PM_test_camera(aspect_ratio);
        auto skybox = PM_test_skybox();
        BVHNode world_bvh(world, cam.shutter_open(), cam.shutter_close());

        // photon map and integrator
        auto photon_map = std::make_shared<PhotonMap>(photon_map_size * 1.2);
        auto integrator = PhotonMappingIntegrator(world, skybox, photon_map);

        // generate photon map
        Vector3d origin, direction, power = Vector3d(6, 6, 6);
        double power_scale;
        while (photon_map->get_photon_num() < photon_map_size) {
            if (photon_map->get_photon_num() % 100000 == 0)
                std::cerr << "Finished " << photon_map->get_photon_num() << " photons." << std::endl;
            light->generate_photon(origin, direction, power_scale);
            Ray ray(origin, direction);
            integrator.trace_photon(ray, 10, power_scale * power);
        }
        while (photon_map->get_photon_num() < photon_map_size * 1.2) {
            if (photon_map->get_photon_num() % 50000 == 0)
                std::cerr << "Finished " << photon_map->get_photon_num() << " photons." << std::endl;
            light->generate_photon(origin, direction, power_scale);
            Ray ray(origin, direction);
            integrator.trace_photon_caustic(ray, 10,
                                            power_scale * power * Vector3d(0.87, 0.49, 0.173) * 12);
        }
        photon_map->balance();

        // render
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