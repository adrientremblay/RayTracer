//
// Created by adrien on 13/03/23.
//

#include "AreaLight.h"

AreaLight::AreaLight(Eigen::Vector3f diffuseColor, Eigen::Vector3f specularColor, Eigen::Vector3f p1, Eigen::Vector3f p2, Eigen::Vector3f p3, Eigen::Vector3f p4,
bool useCenter) : Light(diffuseColor, specularColor), p1(p1), p2(p2), p3(p3), p4(p4), useCenter(useCenter) {
    Eigen::Vector3f v1 = p2 - p1;
    Eigen::Vector3f v2 = p3 - p1;
    normal = v1.cross(v2).normalized();

    Eigen::Vector3f p1p2 = (p2 - p1);
    Eigen::Vector3f p2p3 = (p3 - p2);
    center = p1 + (0.5*p1p2) + (0.5*p2p3);
}

Eigen::Vector3f AreaLight::getDirection(const HitRecord& hitRecord) const {
    return (center - hitRecord.point).normalized();
}

Eigen::Vector3f AreaLight::getPosition() const {
    return center;
}
