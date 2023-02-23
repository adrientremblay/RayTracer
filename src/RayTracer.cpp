//
// Created by adrien on 21/02/23.
//

#include <iostream>
#include "RayTracer.h"
#include "simpleppm.h"

RayTracer::RayTracer(nlohmann::json& j) {

}

void RayTracer::run() {
    // drawing an image
    const int image_width = 256;
    const int image_height = 256;

    std::vector<double> buffer(3*image_width*image_height);
    for (int j = 0 ; j < image_height ; j++) {
        std::cerr << "\rScanlines remaining: " << image_height - j << std::flush;
        for (int i = 0 ; i < image_width ; i++){
            buffer[3*j*image_width+3*i+0] = double(i) / (image_width - 1);
            buffer[3*j*image_width+3*i+1] = double(j) / (image_height - 1);
            buffer[3*j*image_width+3*i+2] = 0.25;
        }
    }
    std::cerr << "\nDone.\n";

    save_ppm("adrien.ppm", buffer, image_width, image_height);
}