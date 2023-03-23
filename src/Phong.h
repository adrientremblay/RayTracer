//
// Created by adrien on 10/03/23.
//

#ifndef RAYTRACER_PHONG_H
#define RAYTRACER_PHONG_H

#include "Material.h"

class Phong : public Material {
public:
    Phong(const Eigen::Vector3f &ambientColor, const Eigen::Vector3f &diffuseColor,
                 const Eigen::Vector3f &specularColor, float ambientCoeff, float diffuseCoeff, float specularCoeff, float phongCoeff);

    const float shadowAcneBias = 1e-4;

    virtual Eigen::Vector3f color(const Ray& rayIn, const HitRecord& hitRecord, const std::vector<Light*>& lights, const HittableList& world) const override;
};


#endif //RAYTRACER_PHONG_H
