//
// Created by adrien on 28/02/23.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include <vector>
#include "geometry/Hittable.h"
#include "util.h"
#include "lights/PointLight.h"

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

    //virtual bool scatter(const Ray& rayIn, const HitRecord& hitRecord, Eigen::Vector3f& attenuation, Ray& scattered) const = 0;

    virtual Eigen::Vector3f color(const Ray& rayIn, const HitRecord& hitRecord, const std::vector<Light*>& lights) const = 0;
};


#endif //RAYTRACER_MATERIAL_H
