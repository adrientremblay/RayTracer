//
// Created by adrien on 28/02/23.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "Hittable.h"
#include "util.h"

class Material {
public:
    virtual bool scatter(const Ray& rayIn, const HitRecord& hitRecord, Eigen::Vector3f attenuation, Ray& scattered) const = 0;
};


#endif //RAYTRACER_MATERIAL_H
