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
#include "Camera.h"

class RayTracer {
private:
    HittableList world;
    std::vector<Light*> lights;
    std::vector<Camera> cameras;
public:
    RayTracer(nlohmann::json& j);
    void run();
private:
    Eigen::Vector3f rayColor(const Ray& ray, int depth, const Camera& camera);
};


#endif //RAYTRACER_RAYTRACER_H
