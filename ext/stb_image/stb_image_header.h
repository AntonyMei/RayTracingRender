//
// Created by meiyixuan on 2021-12-18.
// This file imports stb_image.h
//

#ifndef PROJECT_STB_IMAGE_HEADER_H
#define PROJECT_STB_IMAGE_HEADER_H

// Disable pedantic warnings for this external library.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
#pragma warning (push, 0)
#endif

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

// Restore warning levels.
#ifdef _MSC_VER
// Microsoft Visual C++ Compiler
#pragma warning (pop)
#endif

#endif //PROJECT_STB_IMAGE_HEADER_H
