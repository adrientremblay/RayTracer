//
// Created by adrien on 10/03/23.
//

#include "PointLight.h"

PointLight::PointLight(Eigen::Vector3f center, Eigen::Vector3f diffuseColor, Eigen::Vector3f specularColor) : center(center), Light(diffuseColor, specularColor) {}

Eigen::Vector3f PointLight::getDirection(const HitRecord& hitRecord) const {
    return (center - hitRecord.point).normalized();
}
