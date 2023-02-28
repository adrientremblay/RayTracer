//
// Created by adrien on 28/02/23.
//

#ifndef RAYTRACER_METAL_H
#define RAYTRACER_METAL_H

#include "Material.h"

class Metal : public Material {
public:
    Eigen::Vector3f albedo;

    Metal(const Eigen::Vector3f albedo);

    virtual bool scatter(const Ray& rayIn, const HitRecord& hitRecord, Eigen::Vector3f attenuation, Ray& scattered) const override;
};


#endif //RAYTRACER_METAL_H
