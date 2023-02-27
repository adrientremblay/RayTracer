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
    // Image
    const float aspect_ratio = 16.0 / 9.0;
    const int image_width = 400; // px
    const int image_height = static_cast<int>(image_width / aspect_ratio); // px

    // Camera
    const float viewport_height = 2.0;
    const float viewport_width = aspect_ratio * viewport_height;
    const float focal_length = 1.0;

    const Eigen::Vector3f origin = Eigen::Vector3f(0, 0, 0);
    const Eigen::Vector3f horizontal = Eigen::Vector3f(viewport_width, 0, 0);
    const Eigen::Vector3f vertical = Eigen::Vector3f(0, viewport_height, 0);
    const Eigen::Vector3f lower_left_corner = origin - horizontal / 2 - vertical / 2 - Eigen::Vector3f(0, 0, focal_length);

    // Render
    std::vector<double> buffer(3*image_width*image_height);
    for (int j = 0 ; j < image_height ; j++) {
        std::cerr << "\rScanlines remaining: " << image_height - j << std::flush;
        for (int i = 0 ; i < image_width ; i++){
            float u = float(i)  / (image_width-1);
            float v = float(j)  / (image_height-1);

            Ray ray = Ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);

            Eigen::Vector3f pixel_color = rayColor(ray);

            buffer[3*j*image_width+3*i+0] = pixel_color.x();
            buffer[3*j*image_width+3*i+1] = pixel_color.y();
            buffer[3*j*image_width+3*i+2] = pixel_color.z();
        }
    }
    std::cerr << "\nDone.\n";

    save_ppm("adrien.ppm", buffer, image_width, image_height);
}

Eigen::Vector3f RayTracer::rayColor(Ray& ray) {
    Eigen::Vector3f unit_direction = ray.getDirection().normalized();
    double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Eigen::Vector3f(1.0, 1.0, 1.0) + t * Eigen::Vector3f(0.5, 0.7, 1.0);
}