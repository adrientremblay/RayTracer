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

class RayTracer {
private:
    HittableList world;
public:
    RayTracer(nlohmann::json& j);
    void run();
private:
    Eigen::Vector3f rayColor(const Ray& ray);
};


#endif //RAYTRACER_RAYTRACER_H
