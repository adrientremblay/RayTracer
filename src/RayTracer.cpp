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
#include "Camera.h"
#include "materials/Lambertian.h"
#include "materials/Metal.h"

RayTracer::RayTracer(nlohmann::json& j) {
    std::shared_ptr<Lambertian> material_ground = std::make_shared<Lambertian>(Eigen::Vector3f(0.8, 0.8, 0.0));
    std::shared_ptr<Lambertian> material_center = std::make_shared<Lambertian>(Eigen::Vector3f(0.7, 0.3, 0.3));
    std::shared_ptr<Metal> material_left = std::make_shared<Metal>(Eigen::Vector3f(0.8, 0.8, 0.8));
    std::shared_ptr<Metal> material_right = std::make_shared<Metal>(Eigen::Vector3f(0.8, 0.6, 0.2));

    world.add(std::make_shared<Sphere>(Eigen::Vector3f(0,-100.5,-1), 100,  material_ground));
    world.add(std::make_shared<Sphere>(Eigen::Vector3f(0, 0, -1), 0.5, material_center));
    world.add(std::make_shared<Sphere>(Eigen::Vector3f(-1.0, 0, -1), 0.5, material_left));
    world.add(std::make_shared<Sphere>(Eigen::Vector3f(1.0, 0, -1), 0.5, material_right));
}

void RayTracer::run() {
    // Image
    const int image_width = 400; // px
    const int image_height = static_cast<int>(image_width / aspect_ratio); // px
    const int samples_per_pixel = 10; // turn this up for good renders
    const double pp_scale = 1.0 / samples_per_pixel;
    const double max_depth = 10;

    // Camera
    Camera camera;

    // Render
    std::vector<double> buffer(3*image_width*image_height);
    for (int j = 0 ; j < image_height ; j++) {
        std::cerr << "\rScanlines remaining: " << image_height - j << std::flush;
        for (int i = 0 ; i < image_width ; i++){
            Eigen::Vector3f pixel_color(0, 0, 0);

            for (int s = 1 ; s <= samples_per_pixel ; s++) {
                double u = double(i + random_double())  / (image_width-1);
                double v = double(j + random_double())  / (image_height-1);
                Ray ray = camera.getRay(u, v);
                pixel_color += rayColor(ray, max_depth);
            }

            // scale and gamma correct and clamp
            const double r = clamp(sqrt(pixel_color.x() * pp_scale), 0.0, 0.999);
            const double g = clamp(sqrt(pixel_color.y() * pp_scale), 0.0, 0.999);
            const double b = clamp(sqrt(pixel_color.z() * pp_scale), 0.0, 0.999);

            const int row = 3 * (image_height - j - 1) * image_width;
            const int col = 3 * i;
            const int cell = row + col;
            buffer[cell + 0] = r;
            buffer[cell + 1] = g;
            buffer[cell + 2] = b;
        }
    }
    std::cerr << "\nDone.\n";

    save_ppm("adrien.ppm", buffer, image_width, image_height);
}

Eigen::Vector3f RayTracer::rayColor(const Ray& ray, int depth) {
    if (depth <= 0)
        return Eigen::Vector3f(0, 0, 0);

    HitRecord hitRecord;
    if (world.hit(ray, 0.001, infinity, hitRecord)) {
        Ray scattered;
        Eigen::Vector3f attenuation;

        if (hitRecord.material->scatter(ray, hitRecord, attenuation, scattered)) {
            return attenuation + rayColor(scattered, depth-1);
        }

        return Eigen::Vector3f(0, 0, 0);
    }

    Eigen::Vector3f unit_direction = ray.getDirection().normalized();
    double blueness = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - blueness) * Eigen::Vector3f(1.0, 1.0, 1.0) + blueness * Eigen::Vector3f(0.5, 0.7, 1.0);
}