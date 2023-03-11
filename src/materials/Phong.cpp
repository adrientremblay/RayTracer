//
// Created by adrien on 10/03/23.
//

#include "Phong.h"


Phong::Phong(const Eigen::Vector3f &ambientColor, const Eigen::Vector3f &diffuseColor,
                   const Eigen::Vector3f &specularColor, float ambientCoeff, float diffuseCoeff, float specularCoeff) : Material(ambientColor, diffuseColor, specularColor, ambientCoeff, diffuseCoeff, specularCoeff) {
}

Eigen::Vector3f Phong::color(const Ray& rayIn, const HitRecord& hitRecord) const {
    Eigen::Vector3f light_direction(0.5, 0.5, 0.5);
    float pc = 10;

    float coeff = -light_direction.dot(hitRecord.normal);

    Eigen::Vector3f ambient = ambientCoeff * ambientColor;
    Eigen::Vector3f diffuse = diffuseCoeff * coeff * diffuseColor;
    Eigen::Vector3f specular = specularCoeff * pow(-(rayIn.getDirection().dot(reflect_vector(light_direction, hitRecord.normal))), pc) * specularColor;

    return ambient + diffuse + specular;
}
