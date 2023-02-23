//
// Created by adrien on 21/02/23.
//

#include <iostream>
#include "RayTracer.h"
#include "simpleppm.h"
#include <Eigen/Dense>

RayTracer::RayTracer(nlohmann::json& j) {

}

void RayTracer::run() {
    Eigen::MatrixXd m(2, 2);
    m(0, 0) = 3;
    m(1, 0) = 2.5;
    m(0, 1) = -1;
    m(1, 1) = m(1, 0) + m(0, 1);

    std::cerr << m << std::endl;

    // drawing an image
    const int image_width = 256;
    const int image_height = 256;

    std::vector<double> buffer(3*image_width*image_height);
    for (int j = 0 ; j < image_height ; j++) {
        //std::std << "\rScanlines remaining: " << image_height - j << std::flush;
        for (int i = 0 ; i < image_width ; i++){
            buffer[3*j*image_width+3*i+0] = double(i) / (image_width - 1);
            buffer[3*j*image_width+3*i+1] = double(j) / (image_height - 1);
            buffer[3*j*image_width+3*i+2] = 0.25;
        }
    }
    //std::cerr << "\nDone.\n";

    save_ppm("adrien.ppm", buffer, image_width, image_height);
}