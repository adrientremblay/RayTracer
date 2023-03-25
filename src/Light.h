//
// Created by adrien on 13/03/23.
//

#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H

#include "Eigen/Dense"
#include "Hittable.h"

class Light {
public:
    Eigen::Vector3f diffuseColor;
    Eigen::Vector3f specularColor;

    Light(Eigen::Vector3f diffuseColor, Eigen::Vector3f specularColor);

    virtual Eigen::Vector3f getDirection(const HitRecord& hitRecord) const = 0;
    virtual Eigen::Vector3f getPosition() const = 0;
};

#endif //RAYTRACER_LIGHT_H
