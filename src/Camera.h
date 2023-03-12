//
// Created by adrien on 28/02/23.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "util.h"
#include "Eigen/Dense"
#include "Ray.h"

const double focal_length = 1.0;

class Camera {
public:
    Eigen::Vector3f origin;
    Eigen::Vector3f lowerLeftCorner;
    Eigen::Vector3f horizontal;
    Eigen::Vector3f vertical;
    double aspectRatio;
    double fov;
    double imageWidth;
    double imageHeight;
    Camera(double fov, double imageHeight, double imageWidth);

    Ray getRay(double u, double v) const;
};


#endif //RAYTRACER_CAMERA_H
