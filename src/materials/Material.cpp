//
// Created by adrien on 28/02/23.
//

#include "Material.h"

Material::Material(const Eigen::Vector3f &ambientColor, const Eigen::Vector3f &diffuseColor,
                   const Eigen::Vector3f &specularColor, float ambientCoeff, float diffuseCoeff, float specularCoeff)
        : ambientColor(ambientColor), diffuseColor(diffuseColor), specularColor(specularColor),
          ambientCoeff(ambientCoeff), diffuseCoeff(diffuseCoeff), specularCoeff(specularCoeff) {}