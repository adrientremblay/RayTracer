//
// Created by adrien on 22/02/23.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "Eigen/Dense"

class Ray {
private:
    Eigen::Vector3f origin;
    Eigen::Vector3f direction;
public:
    Ray();
    Ray(const Eigen::Vector3f& origin, const Eigen::Vector3f& direction);

    Eigen::Vector3f getOrigin() const;
    Eigen::Vector3f getDirection() const;

    Eigen::Vector3f at(double t) const;
};


#endif //RAYTRACER_RAY_H
