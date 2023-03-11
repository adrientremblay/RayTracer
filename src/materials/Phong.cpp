//
// Created by adrien on 10/03/23.
//

#include "Phong.h"

Phong::Phong(const Eigen::Vector3f &ambientColor, const Eigen::Vector3f &diffuseColor,
                   const Eigen::Vector3f &specularColor, float ambientCoeff, float diffuseCoeff, float specularCoeff) : Material(ambientColor, diffuseColor, specularColor, ambientCoeff, diffuseCoeff, specularCoeff) {
}

Eigen::Vector3f Phong::color(const Ray& rayIn, const HitRecord& hitRecord, const std::vector<PointLight>& lights) const {

    // todo: shitty code
    PointLight light = lights.at(0);

    Eigen::Vector3f light_direction = hitRecord.point - light.center;
    float pc = 10;

    float coeff = -light_direction.dot(hitRecord.normal);

    Eigen::Vector3f ambient = vector_multiply(light.ambientColor,  ambientCoeff * ambientColor);
    Eigen::Vector3f diffuse = vector_multiply(light.diffuseColor, diffuseCoeff * coeff * diffuseColor);
    Eigen::Vector3f specular = specularCoeff * pow(-(rayIn.getDirection().dot(reflect_vector(light_direction, hitRecord.normal))), pc) * specularColor;

    return ambient + diffuse + specular;
}
