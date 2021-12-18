//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_TEXTURE_H
#define PROJECT_TEXTURE_H

class Texture {
public:
    virtual Color uv_color(double u, double v, const Point &p) const = 0;
};

#endif //PROJECT_TEXTURE_H
