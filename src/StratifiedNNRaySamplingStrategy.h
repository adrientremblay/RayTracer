//
// Created by adrien on 24/03/23.
//

#ifndef RAYTRACER_STRATIFIEDNNRAYSAMPLINGSTRATEGY_H
#define RAYTRACER_STRATIFIEDNNRAYSAMPLINGSTRATEGY_H

#include "RaySamplingStrategy.h"

class StratifiedNNRaySamplingStrategy : public RaySamplingStrategy {
public:
    virtual std::vector<RayCoord> sampleRayCoords(double pixel_bottom_left_x, double pixel_bottom_left_y, const std::vector<int>& raysPerPixel) const override;
};


#endif //RAYTRACER_STRATIFIEDNNRAYSAMPLINGSTRATEGY_H
