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
#include "materials/Phong.h"

RayTracer::RayTracer(nlohmann::json& j) {
    /*
    std::shared_ptr<Lambertian> material_ground = std::make_shared<Lambertian>(Eigen::Vector3f(0.8, 0.8, 0.0));
    std::shared_ptr<Lambertian> material_center = std::make_shared<Lambertian>(Eigen::Vector3f(0.7, 0.3, 0.3));
    std::shared_ptr<Metal> material_left = std::make_shared<Metal>(Eigen::Vector3f(0.8, 0.8, 0.8), 0.3);
    std::shared_ptr<Metal> material_right = std::make_shared<Metal>(Eigen::Vector3f(0.8, 0.6, 0.2), 1.0);
     */

    /*
    world.add(std::make_shared<Sphere>(Eigen::Vector3f(0,-100.5,-1), 100,  material_ground));
    world.add(std::make_shared<Sphere>(Eigen::Vector3f(0, 0, -1), 0.5, material_center));
    world.add(std::make_shared<Sphere>(Eigen::Vector3f(-1.0, 0, -1), 0.5, material_left));
    world.add(std::make_shared<Sphere>(Eigen::Vector3f(1.0, 0, -1), 0.5, material_right));
     */

    nlohmann::json::iterator it;
    if ((it = j.find("geometry")) != j.end()) {
        for (auto& [key, geometry] : (*it).items()) {
            // Material

            std::vector<float> ac = *geometry.find("ac");
            Eigen::Vector3f ambient_color = Eigen::Vector3f(ac.at(0), ac.at(1), ac.at(2));

            std::vector<float> dc = *geometry.find("dc");
            Eigen::Vector3f diffuse_color = Eigen::Vector3f(dc.at(0), dc.at(1), dc.at(2));

            std::vector<float> sc = *geometry.find("sc");
            Eigen::Vector3f specular_color = Eigen::Vector3f(sc.at(0), sc.at(1), sc.at(2));

            float ambient_coeff = *geometry.find("ka");
            float diffuse_coeff = *geometry.find("kd");
            float specular_coeff = *geometry.find("ks");

            std::shared_ptr<Phong> geometry_material = std::make_shared<Phong>(ambient_color, diffuse_color, specular_color,
                                                                               ambient_coeff, diffuse_coeff, specular_coeff);

            // type specific stuff
            std::string geometry_type = *geometry.find("type");
            if (geometry_type == "sphere") {
                std::vector<float> centre = *geometry.find("centre");
                Eigen::Vector3f centre_vector = Eigen::Vector3f(centre.at(0), centre.at(1), centre.at(2));

                float sphere_radius = *geometry.find("radius");

                world.add(std::make_shared<Sphere>(centre_vector, sphere_radius, geometry_material));
            }
        }
    }

}

void RayTracer::run() {
    // Image
    const int image_width = 400; // px
    const int image_height = static_cast<int>(image_width / aspect_ratio); // px
    const int samples_per_pixel = 20; // turn this up for good renders
    const double pp_scale = 1.0 / samples_per_pixel;
    const double max_depth = 20;

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
        return hitRecord.material->color(ray, hitRecord);

        /*
        Ray scattered;
        Eigen::Vector3f attenuation;
        if (hitRecord.material->scatter(ray, hitRecord, attenuation, scattered)) {
            return vector_multiply(attenuation, rayColor(scattered, depth-1));
        }

        return Eigen::Vector3f(0, 0, 0);
        */
    }

    // Background Color
    Eigen::Vector3f unit_direction = ray.getDirection().normalized();
    double blueness = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - blueness) * Eigen::Vector3f(1.0, 1.0, 1.0) + blueness * Eigen::Vector3f(0.5, 0.7, 1.0);
}