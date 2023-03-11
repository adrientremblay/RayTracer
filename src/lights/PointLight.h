//
// Created by adrien on 10/03/23.
//

#ifndef RAYTRACER_POINTLIGHT_H
#define RAYTRACER_POINTLIGHT_H

#include "Eigen/Dense"

class PointLight {
public:
    Eigen::Vector3f center;
    Eigen::Vector3f diffuseColor;
    Eigen::Vector3f ambientColor;

    PointLight(Eigen::Vector3f center, Eigen::Vector3f diffuseColor, Eigen::Vector3f ambientColor);
};


#endif //RAYTRACER_POINTLIGHT_H
