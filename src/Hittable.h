//
// Created by adrien on 27/02/23.
//

#ifndef RAYTRACER_HITTABLE_H
#define RAYTRACER_HITTABLE_H

#include "Eigen/Dense"
#include "Ray.h"
#include <memory>

class Material;

struct HitRecord {
    Eigen::Vector3f point;
    Eigen::Vector3f normal;
    Eigen::Vector3f outwardNormal;
    std::shared_ptr<Material> material;
    double t;
    bool rayFromOutside;

    inline void setFaceNormal(const Ray& ray, const Eigen::Vector3f& norm) {
        normal = norm;

        rayFromOutside = ray.getDirection().dot(norm) < 0;
        outwardNormal = rayFromOutside ? norm : -norm;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const = 0;
};


#endif //RAYTRACER_HITTABLE_H
