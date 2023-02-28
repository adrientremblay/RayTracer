//
// Created by adrien on 28/02/23.
//

#ifndef RAYTRACER_LAMBERTIAN_H
#define RAYTRACER_LAMBERTIAN_H

#include "Material.h"

class Lambertian : public Material {
public:
    Eigen::Vector3f albedo; // color???

    Lambertian(const Eigen::Vector3f albedo);
    virtual bool scatter(const Ray& rayIn, const HitRecord& hitRecord, Eigen::Vector3f attenuation, Ray& scattered) const override;
};


#endif //RAYTRACER_LAMBERTIAN_H
