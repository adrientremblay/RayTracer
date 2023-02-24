//
// Created by adrien on 22/02/23.
//

#include "Ray.h"

Ray::Ray(const Eigen::Vector3f& origin, const Eigen::Vector3f& direction) : origin(origin), direction(direction) {

}

Eigen::Vector3f Ray::at(double t) const {
    return origin + t * direction;
}

Eigen::Vector3f Ray::getOrigin() {
    return origin;
}

Eigen::Vector3f Ray::getDirection() {
   return direction;
}
