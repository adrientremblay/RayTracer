//
// Created by adrien on 21/02/23.
//

#include <iostream>
#include "RayTracer.h"
#include "simpleppm.h"
#include <Eigen/Dense>
#include "RayTracer.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

RayTracer::RayTracer(nlohmann::json& j) {

}

void RayTracer::run() {
    MatrixXd m = MatrixXd::Random(3,3);
    std::cerr << m << std::endl;

    m = (m + MatrixXd::Constant(3,3,1.2)) * 50;

    std::cerr << MatrixXd::Constant(3,3,1.2) * 50 << std::endl;

    std::cout << "m =" << std::endl << m << std::endl;
    VectorXd v(3);
    v << 1, 2, 3;
    std::cout << "m * v =" << std::endl << m * v << std::endl;

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

Eigen::Vector3f RayTracer::rayColor(const Ray &ray) {
    Eigen::Vector3f unit_direction = ray / ray.
}