//
// Created by adrien on 13/03/23.
//

#include "AreaLight.h"
#include "util.h"

AreaLight::AreaLight(Eigen::Vector3f diffuseColor, Eigen::Vector3f specularColor, Eigen::Vector3f p1, Eigen::Vector3f p2, Eigen::Vector3f p3, Eigen::Vector3f p4,
bool useCenter) : Light(diffuseColor, specularColor), p1(p1), p2(p2), p3(p3), p4(p4), useCenter(useCenter) {
    Eigen::Vector3f v1 = p2 - p1;
    Eigen::Vector3f v2 = p3 - p1;
    normal = v1.cross(v2).normalized();

    p1p2 = (p2 - p1);
    p2p3 = (p3 - p2);
    center = p1 + (0.5*p1p2) + (0.5*p2p3);
}

Eigen::Vector3f AreaLight::getDirection(const HitRecord& hitRecord) const {
    if (useCenter)
        return (center - hitRecord.point).normalized();

    Eigen::Vector3f random_point_on_rect = p1 + (random_double()*p1p2) + (random_double()*p2p3);
    return (random_point_on_rect - hitRecord.point).normalized();
}

Eigen::Vector3f AreaLight::getPosition() const {
    return center;
}
