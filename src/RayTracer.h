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
#include "RaySamplingStrategy.h"

#define GAMMA_CORRECTION_ENABLED false

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
    Eigen::Vector3f rayTrace(const Ray& ray, const Camera& camera);
    Eigen::Vector3f pathTrace(const Ray& ray, int depth, const Camera& camera, bool& hitNothing);
    static inline float gammaCorrect(float color);
};

#endif //RAYTRACER_RAYTRACER_H
