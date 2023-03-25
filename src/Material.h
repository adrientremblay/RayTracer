//
// Created by adrien on 28/02/23.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include <vector>
#include "Hittable.h"
#include "util.h"
#include "PointLight.h"
#include "HittableList.h"
#include "AreaLight.h"

class Material {
public:
    Eigen::Vector3f ambientColor;
    Eigen::Vector3f diffuseColor;
    Eigen::Vector3f specularColor;

    float ambientCoeff;
    float diffuseCoeff;
    float specularCoeff;
    float phongCoeff;

    Material(const Eigen::Vector3f &ambientColor, const Eigen::Vector3f &diffuseColor,
             const Eigen::Vector3f &specularColor, float ambientCoeff, float diffuseCoeff, float specularCoeff,
             float phongCoeff);

    virtual Eigen::Vector3f color(const Ray& rayIn, const HitRecord& hitRecord, const std::vector<PointLight>& pointLights, const std::vector<AreaLight>& areaLights, const HittableList& world) const = 0;
};


#endif //RAYTRACER_MATERIAL_H
