//
// Created by adrien on 28/02/23.
//

#include "Camera.h"

Camera::Camera() {
    origin = Eigen::Vector3f(0, 0, 0);
    horizontal = Eigen::Vector3f(viewport_width, 0, 0);
    vertical = Eigen::Vector3f(0, viewport_height, 0);
    lowerLeftCorner = origin - horizontal / 2 - vertical / 2 - Eigen::Vector3f(0, 0, focal_length);
}

Ray Camera::getRay(double u, double v) const {
    return Ray(origin, lowerLeftCorner + u*horizontal + v*vertical - origin);
}
