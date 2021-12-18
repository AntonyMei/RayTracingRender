//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_SKYBOX_H
#define PROJECT_SKYBOX_H

class Skybox {
public:
    virtual Color get_color(Ray r) const = 0;
};

#endif //PROJECT_SKYBOX_H
