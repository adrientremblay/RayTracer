//
// Created by adrien on 27/02/23.
//

#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H

#include "Eigen/Dense"
#include "Ray.h"

struct HitRecord {
    Eigen::Vector3f point;
    Eigen::Vector3f normal;
    double t;
};

class Hittable {
public:
    virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const = 0;
};


#endif //RAYTRACER_HITTABLE_H
