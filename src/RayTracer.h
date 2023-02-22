//
// Created by adrien on 21/02/23.
//

#ifndef RAYTRACER_RAYTRACER_H
#define RAYTRACER_RAYTRACER_H

#include "json.hpp"

class RayTracer {
public:
    RayTracer(nlohmann::json& j);
    void run();
};


#endif //RAYTRACER_RAYTRACER_H
