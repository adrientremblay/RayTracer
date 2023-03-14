//
// Created by adrien on 13/03/23.
//

#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H

#include "Eigen/Dense"

class Light {
public:
    Eigen::Vector3f diffuseColor;
    Eigen::Vector3f ambientColor;

    Light(Eigen::Vector3f diffuseColor, Eigen::Vector3f ambientColor);
};

#endif //RAYTRACER_LIGHT_H
