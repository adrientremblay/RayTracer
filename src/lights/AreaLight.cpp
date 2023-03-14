//
// Created by adrien on 13/03/23.
//

#include "AreaLight.h"

AreaLight::AreaLight(Eigen::Vector3f diffuseColor, Eigen::Vector3f ambientColor, Eigen::Vector3f p1, Eigen::Vector3f p2, Eigen::Vector3f p3, Eigen::Vector3f p4,
bool useCenter) : Light(diffuseColor, ambientColor), p1(p1), p2(p2), p3(p3), p4(p4), useCenter(useCenter) {

}

Eigen::Vector3f AreaLight::getDirection(const HitRecord& hitRecord) const {
    return Eigen::Vector3f (0,0,0);
}