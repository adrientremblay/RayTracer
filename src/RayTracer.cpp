//
// Created by adrien on 21/02/23.
//

#include <iostream>
#include "RayTracer.h"

RayTracer::RayTracer(nlohmann::json& j) {

}

void RayTracer::run() {
    // drawing an image
    const int image_width = 256;
    const int image_height = 256;

    std::cout << "P3\n" << image_width << image_height << "\n255\n";

    for (int i = 0  ; i < image_height ; i++) {
        for (int j = 0 ; j < image_width ; j++) {
            double r = double(i) / (image_width - 1);
            double g = double(j) / (image_width - 1);
            double b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}