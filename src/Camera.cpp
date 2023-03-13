//
// Created by adrien on 28/02/23.
//

#include "Camera.h"

Camera::Camera(double fov, double imageHeight, double imageWidth, Eigen::Vector3f lookat, Eigen::Vector3f up, Eigen::Vector3f centre, Eigen::Vector3f ai, Eigen::Vector3f bkc) :
fov(fov), imageWidth(imageWidth), imageHeight(imageHeight), lookat(lookat), up(up), centre(centre), ai(ai), bkc(bkc) {
    double theta = degreesToRadians(fov);
    double h = tan(theta / 2);

    aspectRatio = imageHeight / imageWidth;

    auto viewport_height = 2.0 * h;
    auto viewport_width = aspectRatio * viewport_height;

    horizontal = Eigen::Vector3f(viewport_width, 0, 0);
    vertical = Eigen::Vector3f(0, viewport_height, 0);
    lowerLeftCorner = centre - horizontal / 2 - vertical / 2 - Eigen::Vector3f(0, 0, focal_length);
}

Ray Camera::getRay(double u, double v) const {
    return Ray(centre, (lowerLeftCorner + u*horizontal + v*vertical - centre).normalized());
}
