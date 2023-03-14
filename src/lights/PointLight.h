//
// Created by adrien on 10/03/23.
//

#ifndef RAYTRACER_POINTLIGHT_H
#define RAYTRACER_POINTLIGHT_H

#include "Eigen/Dense"
#include "Light.h"

class PointLight : public Light {
public:
    Eigen::Vector3f center;

    PointLight(Eigen::Vector3f center, Eigen::Vector3f diffuseColor, Eigen::Vector3f ambientColor);

    virtual Eigen::Vector3f getDirection(const HitRecord& hitRecord) const override;
};


#endif //RAYTRACER_POINTLIGHT_H
