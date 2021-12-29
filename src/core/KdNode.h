//
// Created by meiyixuan on 2021-12-29.
//

#ifndef PROJECT_KDNODE_H
#define PROJECT_KDNODE_H

struct KdNode {
    Photon p;
    std::shared_ptr<KdNode> left;
    std::shared_ptr<KdNode> right;
};

#endif //PROJECT_KDNODE_H
