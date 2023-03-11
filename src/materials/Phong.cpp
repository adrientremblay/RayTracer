//
// Created by adrien on 10/03/23.
//

#include <iostream>
#include "Phong.h"

Phong::Phong(const Eigen::Vector3f &ambientColor, const Eigen::Vector3f &diffuseColor,
                   const Eigen::Vector3f &specularColor, float ambientCoeff, float diffuseCoeff, float specularCoeff, float phongCoeff) : Material(ambientColor, diffuseColor, specularColor, ambientCoeff, diffuseCoeff, specularCoeff, phongCoeff) {
}

Eigen::Vector3f Phong::color(const Ray& rayIn, const HitRecord& hitRecord, const std::vector<PointLight>& lights) const {
    return Eigen::Vector3f(1, 0, 0);

    // todo: get lights properly
    PointLight light = lights.at(0);

    Eigen::Vector3f light_direction = (light.center - hitRecord.point).normalized();
    Eigen::Vector3f view_direction = rayIn.getDirection().normalized();
    Eigen::Vector3f halfway_vector = (light_direction + view_direction).normalized();

    Eigen::Vector3f ambient = vector_multiply(light.ambientColor,  ambientCoeff * ambientColor);
    Eigen::Vector3f diffuse = vector_multiply(light.diffuseColor, diffuseCoeff * light_direction.dot(hitRecord.normal) * diffuseColor);
    Eigen::Vector3f specular = specularCoeff * pow(std::max(view_direction.dot(halfway_vector), 0.0f), phongCoeff) * specularColor;

     return ambient + diffuse + specular;
}
