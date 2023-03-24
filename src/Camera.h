//
// Created by adrien on 28/02/23.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include <vector>
#include "util.h"
#include "Eigen/Dense"
#include "Ray.h"
#include "RaySamplingStrategy.h"

const double focal_length = 1.0;

class Camera {
public:
    RaySamplingStrategy* raySamplingStrategy;

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
    bool antiAliasing;
    bool twoSideRender;

    Camera(double fov, double imageWidth, double imageHeight, Eigen::Vector3f lookat, Eigen::Vector3f up, Eigen::Vector3f centre, Eigen::Vector3f ai, Eigen::Vector3f bkc, std::string filename, bool globalIllumination, const std::vector<int>& raysPerPixel, int maxBounces, float probTerminate, bool antiAliasing, bool twoSideRender);

    std::vector<Ray> sampleRays(double pixel_bottom_left_x, double pixel_bottom_left_y);
private:
    Ray getRay(double ray_x, double ray_y) const;
};


#endif //RAYTRACER_CAMERA_H
