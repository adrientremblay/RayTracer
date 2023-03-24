//
// Created by adrien on 24/03/23.
//

#include "StratifiedNNRaySamplingStrategy.h"
#include "util.h"

std::vector<RayCoord> StratifiedNNRaySamplingStrategy::sampleRayCoords(double pixel_bottom_left_x, double pixel_bottom_left_y, const std::vector<int>& raysPerPixel) const {
    // nxm matrix of stratas per pixel
    int strata_cols;
    int strata_rows;
    double strata_width;
    double strata_height;
    int rays_per_strata = raysPerPixel.at(raysPerPixel.size()-1);
    strata_cols = strata_rows = raysPerPixel.at(0);
    strata_width = strata_height = 1 / strata_cols;

    int num_rays = rays_per_strata * strata_cols * strata_rows;
    std::vector<RayCoord> rayCoords;
    rayCoords.reserve(num_rays);

    for (int strata_x = 0 ; strata_x < strata_cols ; strata_x++) {
        for (int strata_y = 0 ; strata_y < strata_cols ; strata_y++) {
            for (int ray_number = 1 ; ray_number <= rays_per_strata ; ray_number++) {
                double ray_x = double(pixel_bottom_left_x + (strata_x * strata_width) + random_double(strata_width));
                double ray_y = double(pixel_bottom_left_y + (strata_y * strata_height) + random_double(strata_height));

                rayCoords.push_back(RayCoord(ray_x, ray_y));
            }
        }
    }

    return rayCoords;
}
