//
// Created by adrien on 22/02/23.
//

#include "Ray.h"

Ray::Ray() {

}

Ray::Ray(const Eigen::Vector3f& origin, const Eigen::Vector3f& direction, bool initialRay) : origin(origin), direction(direction), initialRay(initialRay) {

}

Eigen::Vector3f Ray::at(double t) const {
    return origin + t * direction;
}

Eigen::Vector3f Ray::getOrigin() const {
    return origin;
}

Eigen::Vector3f Ray::getDirection() const {
   return direction;
}
