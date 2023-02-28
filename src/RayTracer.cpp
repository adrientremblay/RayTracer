//
// Created by adrien on 21/02/23.
//

#include <iostream>
#include "RayTracer.h"
#include "simpleppm.h"
#include <Eigen/Dense>
#include <memory>
#include "Sphere.h"
#include "util.h"

RayTracer::RayTracer(nlohmann::json& j) {
    world.add(std::make_shared<Sphere>(Eigen::Vector3f(0, 0, -1), 0.5));
    world.add(std::make_shared<Sphere>(Eigen::Vector3f(0,-100.5,-1), 100));
}

void RayTracer::run() {
    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400; // px
    const int image_height = static_cast<int>(image_width / aspect_ratio); // px

    // Camera
    const double viewport_height = 2.0;
    const double viewport_width = aspect_ratio * viewport_height;
    const double focal_length = 1.0;

    const Eigen::Vector3f origin = Eigen::Vector3f(0, 0, 0);
    const Eigen::Vector3f horizontal = Eigen::Vector3f(viewport_width, 0, 0);
    const Eigen::Vector3f vertical = Eigen::Vector3f(0, viewport_height, 0);
    const Eigen::Vector3f lower_left_corner = origin - horizontal / 2 - vertical / 2 - Eigen::Vector3f(0, 0, focal_length);

    // Render
    std::vector<double> buffer(3*image_width*image_height);
    for (int j = 0 ; j < image_height ; j++) {
        std::cerr << "\rScanlines remaining: " << image_height - j << std::flush;
        for (int i = 0 ; i < image_width ; i++){
            double u = double(i)  / (image_width-1);
            double v = double(j)  / (image_height-1);

            Ray ray = Ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);

            Eigen::Vector3f pixel_color = rayColor(ray);

            const int row = 3 * (image_height - j - 1) * image_width;
            const int col = 3 * i;
            const int cell = row + col;
            buffer[cell + 0] = pixel_color.x();
            buffer[cell + 1] = pixel_color.y();
            buffer[cell + 2] = pixel_color.z();
        }
    }
    std::cerr << "\nDone.\n";

    save_ppm("adrien.ppm", buffer, image_width, image_height);
}

Eigen::Vector3f RayTracer::rayColor(const Ray& ray) {
    HitRecord hitRecord;
    if (world.hit(ray, 0, infinity, hitRecord)) {
        return 0.5 * (hitRecord.normal + Eigen::Vector3f(1, 1, 1));
    }

    Eigen::Vector3f unit_direction = ray.getDirection().normalized();
    double blueness = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - blueness) * Eigen::Vector3f(1.0, 1.0, 1.0) + blueness * Eigen::Vector3f(0.5, 0.7, 1.0);
}