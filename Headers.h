//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_HEADERS_H
#define PROJECT_HEADERS_H

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

// basic math utilities
#include "src/math/Utils.h"
#include "src/math/Vector3d.h"
#include "src/math/Perlin.h"

// base classes
#include "src/core/Ray.h"
#include "src/core/Pixel.h"
#include "src/core/Camera.h"
#include "src/core/AABB.h"
#include "src/core/Texture.h"
#include "src/core/Material.h"
#include "src/core/Hit.h"
#include "src/core/Hittable.h"
#include "src/core/Accelerator.h"
#include "src/core/Light.h"
#include "src/core/Skybox.h"
#include "src/core/Integrator.h"

// derived classes
// camera
#include "src/camera/SimpleCamera.h"
// texture
#include "src/texture/ColorTexture.h"
#include "src/texture/CheckerTexture.h"
#include "src/texture/PerlinTexture.h"
#include "src/texture/MarbleTexture.h"
#include "src/texture/ImageTexture.h"
// material
#include "src/material/Lambertian.h"
#include "src/material/Metal.h"
#include "src/material/Dielectric.h"
// accelerator
#include "src/accelerator/HittableList.h"
#include "src/accelerator/BVH.h"
// hittable
#include "src/hittable/Sphere.h"
#include "src/hittable/MovingSphere.h"
#include "src/hittable/Rectangle.h"
#include "src/hittable/Box.h"
// light
#include "src/light/DiffuseLight.h"
// skybox
#include "src/skybox/SimpleSkybox.h"
#include "src/skybox/ConstantSkybox.h"
// integrator
#include "src/integrator/PathTracingIntegrator.h"

#endif //PROJECT_HEADERS_H
