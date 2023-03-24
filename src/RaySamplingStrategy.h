//
// Created by adrien on 24/03/23.
//

#ifndef RAYTRACER_RAYSAMPLINGSTRATEGY_H
#define RAYTRACER_RAYSAMPLINGSTRATEGY_H

#include "Ray.h"
#include <vector>

struct RayCoord {
    double x;
    double y;

    RayCoord() : x(0), y(0) {
    }

    RayCoord(double x, double y) : x(x), y(y) {
    }
};

class RaySamplingStrategy {
public:
    virtual std::vector<RayCoord> sampleRayCoords(double pixel_bottom_left_x, double pixel_bottom_left_y, const std::vector<int>& raysPerPixel) const = 0;
};


#endif //RAYTRACER_RAYSAMPLINGSTRATEGY_H
