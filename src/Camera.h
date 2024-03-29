//
// Created by adrien on 28/02/23.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include <vector>
#include "util.h"
#include "Eigen/Dense"
#include "Ray.h"
#include "HittableList.h"
#include "Light.h"
#include "PointLight.h"
#include "AreaLight.h"

const double focal_length = 1.0;

enum RaySamplingStrat {
    RANDOM,
    NN,
    NM
};

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

    // optional
    bool globalIllumination;
    std::vector<int> raysPerPixel;
    int maxBounces;
    float probTerminate;
    bool antiAliasing;
    bool twoSideRender;

    int cameraNum;

    Camera(double fov, double imageWidth, double imageHeight, Eigen::Vector3f lookat, Eigen::Vector3f up, Eigen::Vector3f centre, Eigen::Vector3f ai, Eigen::Vector3f bkc, std::string filename, bool globalIllumination, const std::vector<int>& raysPerPixel, int maxBounces, float probTerminate, bool antiAliasing, bool twoSideRender);

    void sampleRays(double pixel_bottom_left_x, double pixel_bottom_left_y, Eigen::Vector3f& pixelColor, const HittableList& world, const std::vector<PointLight>& pointLights, const std::vector<AreaLight>& areaLights);
private:
    RaySamplingStrat raySamplingStrat;
    void sampleRay(double ray_x, double ray_y, Eigen::Vector3f& pixelColor, const HittableList& world, const std::vector<PointLight>& pointLights, const std::vector<AreaLight>& areaLights, int& successfulRays);
    Eigen::Vector3f rayTrace(const Ray& ray, const HittableList& world, const std::vector<PointLight>& pointLights, const std::vector<AreaLight>& areaLights);
    Eigen::Vector3f pathTrace(const Ray& ray, const HittableList& world, const std::vector<PointLight>& pointLights, const std::vector<AreaLight>& areaLights, int depth, bool& hitNothing);
    Ray getRay(double ray_x, double ray_y) const;
    static int camera_count;
};

#endif //RAYTRACER_CAMERA_H
