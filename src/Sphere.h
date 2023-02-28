//
// Created by adrien on 27/02/23.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include "Hittable.h"
#include "materials/Material.h"

class Sphere : public Hittable {
public:
    Eigen::Vector3f center;
    double radius;
    std::shared_ptr<Material> material;

    Sphere();
    Sphere(Eigen::Vector3f center, double radius, std::shared_ptr<Material> material);

    virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const override;
};


#endif //RAYTRACER_SPHERE_H
