//
// Created by adrien on 21/02/23.
//

#ifndef RAYTRACER_RAYTRACER_H
#define RAYTRACER_RAYTRACER_H

#include "json.hpp"
#include "Eigen/Dense"
#include "Ray.h"

class RayTracer {
public:
    RayTracer(nlohmann::json& j);
    void run();
private:
    Eigen::Vector3f rayColor(Ray& ray);
};


#endif //RAYTRACER_RAYTRACER_H
