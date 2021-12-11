//
// Created by meiyixuan on 2021-12-11.
//

#ifndef PROJECT_MATERIAL_H
#define PROJECT_MATERIAL_H

class Material {
public:
    virtual bool scatter(const Ray &ray_in, const Hit &hit, Color &attenuation,
                         std::vector<Ray> &scattered_rays) const = 0;
};

#endif //PROJECT_MATERIAL_H
