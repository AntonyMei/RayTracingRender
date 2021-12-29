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
#include <algorithm>
#include <memory>
#include <vector>
#include <cmath>
#include <limits>
#include <random>

// multiprocessing / multithreading
#ifdef WINDOWS

#include <omp.h>

#else

#include <unistd.h>
#include <sys/wait.h>

#endif

// external libraries
#include "ext/stb_image/stb_image_header.h"
#include "ext/tinyobjloader/tiny_obj_loader_header.h"

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
#include "src/core/Transform.h"
#include "src/core/ParticipatingMedium.h"
#include "src/core/Photon.h"
#include "src/core/PhotonMap.h"

// derived classes
// camera
#include "src/camera/SimpleCamera.h"
// texture
#include "src/texture/ColorTexture.h"
#include "src/texture/CheckerTexture.h"
#include "src/texture/PerlinTexture.h"
#include "src/texture/MarbleTexture.h"
#include "src/texture/ImageTexture.h"
#include "src/texture/BumpTexture.h"
// material
#include "src/material/Lambertian.h"
#include "src/material/Metal.h"
#include "src/material/Dielectric.h"
#include "src/material/Isotropic.h"
#include "src/material/PBRMaterial.h"
#include "src/material/BumpMaterial.h"
// accelerator
#include "src/accelerator/HittableList.h"
#include "src/accelerator/BVH.h"
// hittable
#include "src/hittable/Sphere.h"
#include "src/hittable/MovingSphere.h"
#include "src/hittable/Rectangle.h"
#include "src/hittable/Box.h"
#include "src/hittable/Triangle.h"
#include "src/hittable/Mesh.h"
#include "src/hittable/PhotonLight.h"
// transform
#include "src/transform/Translate.h"
#include "src/transform/RotateY.h"
// light
#include "src/light/DiffuseLight.h"
// skybox
#include "src/skybox/SimpleSkybox.h"
#include "src/skybox/ConstantSkybox.h"
#include "src/skybox/DirectionalSkybox.h"
#include "src/skybox/TwoLayerSkybox.h"
// integrator
#include "src/integrator/PathTracingIntegrator.h"
#include "src/integrator/PhotonMappingIntegrator.h"
// medium
#include "src/medium/ConstantMedium.h"
// object parser
#include "src/core/ObjectParser.h"
// scenes
#include "src/scenes/MotionBlur.h"
#include "src/scenes/MotionBlurChecker.h"
#include "src/scenes/TwoCheckerSpheres.h"
#include "src/scenes/TwoMarbleSpheres.h"
#include "src/scenes/Earth.h"
#include "src/scenes/SimpleLight.h"
#include "src/scenes/CornellBoxEmpty.h"
#include "src/scenes/CornellBoxTwoBlocks.h"
#include "src/scenes/CornellBox.h"
#include "src/scenes/CornellBoxParticipatingMedia.h"
#include "src/scenes/Book2Final.h"
#include "src/scenes/TestObj.h"
#include "src/scenes/Sponza.h"
#include "src/scenes/TestTriangle.h"
#include "src/scenes/SponzaCrytek.h"
#include "src/scenes/PMTest.h"

#endif //PROJECT_HEADERS_H
