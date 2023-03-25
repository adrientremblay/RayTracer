//
// Created by adrien on 21/02/23.
//

#ifndef RAYTRACER_RAYTRACER_H
#define RAYTRACER_RAYTRACER_H

#include "json.hpp"
#include "Eigen/Dense"
#include "Ray.h"
#include "HittableList.h"
#include "Sphere.h"
#include "PointLight.h"
#include "AreaLight.h"
#include "Camera.h"

class RayTracer {
private:
    HittableList world;
    std::vector<PointLight> pointLights;
    std::vector<AreaLight> areaLights;
    std::vector<Camera> cameras;
public:
    RayTracer(nlohmann::json& j);
    void run();
private:
    static inline float gammaCorrect(float color);
};

#endif //RAYTRACER_RAYTRACER_H
