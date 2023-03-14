//
// Created by adrien on 13/03/23.
//

#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H

#include "Eigen/Dense"
#include "Camera.h"
#include "geometry/Hittable.h"

class Light {
public:
    Eigen::Vector3f diffuseColor;
    Eigen::Vector3f ambientColor;

    Light(Eigen::Vector3f diffuseColor, Eigen::Vector3f ambientColor);

    virtual Eigen::Vector3f getDirection(const HitRecord& hitRecord) const = 0;
};

#endif //RAYTRACER_LIGHT_H
