//
// Created by meiyixuan on 2021-12-18.
//

#ifndef PROJECT_BOX_H
#define PROJECT_BOX_H

class Box : public Hittable {
public:
    Point min_point, max_point;
    // HittableList faces;
};

#endif //PROJECT_BOX_H
