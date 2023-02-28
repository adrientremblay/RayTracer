//
// Created by adrien on 28/02/23.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "util.h"
#include "Eigen/Dense"
#include "Ray.h"

const double aspect_ratio = 16.0 / 9.0;
const double viewport_height = 2.0;
const double viewport_width = aspect_ratio * viewport_height;
const double focal_length = 1.0;

class Camera {
private:
    Eigen::Vector3f origin;
    Eigen::Vector3f lowerLeftCorner;
    Eigen::Vector3f horizontal;
    Eigen::Vector3f vertical;
public:
    Camera();

    Ray getRay(double u, double v) const;
};


#endif //RAYTRACER_CAMERA_H
