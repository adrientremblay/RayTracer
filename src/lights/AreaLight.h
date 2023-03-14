//
// Created by adrien on 13/03/23.
//

#ifndef RAYTRACER_AREALIGHT_H
#define RAYTRACER_AREALIGHT_H

#include "Light.h"

class AreaLight : public Light {
public:
    Eigen::Vector3f p1;
    Eigen::Vector3f p2;
    Eigen::Vector3f p3;
    Eigen::Vector3f p4;
    bool useCenter;

    AreaLight(Eigen::Vector3f diffuseColor, Eigen::Vector3f ambientColor, Eigen::Vector3f p1, Eigen::Vector3f p2, Eigen::Vector3f p3, Eigen::Vector3f p4,
              bool useCenter);

    virtual Eigen::Vector3f getDirection(const HitRecord& hitRecord) const override;
};


#endif //RAYTRACER_AREALIGHT_H
