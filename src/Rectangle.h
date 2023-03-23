//
// Created by adrien on 11/03/23.
//

#ifndef RAYTRACER_RECTANGLE_H
#define RAYTRACER_RECTANGLE_H

#include "Eigen/Dense"
#include "Hittable.h"

class Rectangle : public Hittable {
public:
    Eigen::Vector3f p1;
    Eigen::Vector3f p2;
    Eigen::Vector3f p3;
    Eigen::Vector3f p4;
    std::shared_ptr<Material> material;
    Eigen::Vector3f normal;

    Rectangle(Eigen::Vector3f p1, Eigen::Vector3f p2, Eigen::Vector3f p3, Eigen::Vector3f p4, std::shared_ptr<Material> material);

    virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const override;
};


#endif //RAYTRACER_RECTANGLE_H
