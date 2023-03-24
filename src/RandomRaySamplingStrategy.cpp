//
// Created by adrien on 24/03/23.
//

#include "RandomRaySamplingStrategy.h"
#include "util.h"

std::vector<RayCoord> RandomRaySamplingStrategy::sampleRayCoords(double pixel_bottom_left_x, double pixel_bottom_left_y, const std::vector<int>& raysPerPixel) const {
    int num_rays = raysPerPixel.at(0);

    std::vector<RayCoord> ret(num_rays);

    for (int ray_number = 1 ; ray_number <= num_rays ; ray_number++) {
        double ray_x = double(pixel_bottom_left_x + random_double());
        double ray_y = double(pixel_bottom_left_y + random_double());

        ret.push_back(RayCoord(ray_x, ray_y));
    }

    return ret;
}
