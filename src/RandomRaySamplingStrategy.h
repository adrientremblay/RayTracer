//
// Created by adrien on 24/03/23.
//

#ifndef RAYTRACER_RANDOMRAYSAMPLINGSTRATEGY_H
#define RAYTRACER_RANDOMRAYSAMPLINGSTRATEGY_H

#include "RaySamplingStrategy.h"

class RandomRaySamplingStrategy : public RaySamplingStrategy {
public:
    RandomRaySamplingStrategy() {

    }
    virtual std::vector<RayCoord> sampleRayCoords(double pixel_bottom_left_x, double pixel_bottom_left_y, const std::vector<int>& raysPerPixel) const override;
};


#endif //RAYTRACER_RANDOMRAYSAMPLINGSTRATEGY_H
