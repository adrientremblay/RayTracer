//
// Created by adrien on 28/02/23.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include <vector>
#include "util.h"
#include "Eigen/Dense"
#include "Ray.h"

const double focal_length = 1.0;

class Camera {
public:
    Eigen::Vector3f lowerLeftCorner;
    Eigen::Vector3f horizontal;
    Eigen::Vector3f vertical;
    Eigen::Vector3f lookat;
    Eigen::Vector3f up;
    Eigen::Vector3f centre;
    Eigen::Vector3f ai;
    Eigen::Vector3f bkc;
    double aspectRatio;
    double fov;
    double imageWidth;
    double imageHeight;
    std::string filename;

    bool globalIllumination;
    std::vector<int> raysPerPixel;
    int maxBounces;
    float probTerminate;

    Camera(double fov, double imageHeight, double imageWidth, Eigen::Vector3f lookat, Eigen::Vector3f up, Eigen::Vector3f centre, Eigen::Vector3f ai, Eigen::Vector3f bkc, std::string filename, bool globalIllumination, const std::vector<int>& raysPerPixel, int maxBounces, float probTerminate);

    Ray getRay(double u, double v) const;
};


#endif //RAYTRACER_CAMERA_H
