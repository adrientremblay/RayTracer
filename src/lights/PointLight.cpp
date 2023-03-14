//
// Created by adrien on 10/03/23.
//

#include "PointLight.h"

PointLight::PointLight(Eigen::Vector3f center, Eigen::Vector3f diffuseColor, Eigen::Vector3f ambientColor) : center(center), Light(diffuseColor, ambientColor) {}
