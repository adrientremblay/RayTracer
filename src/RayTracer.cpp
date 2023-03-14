//
// Created by adrien on 21/02/23.
//

#include <iostream>
#include "RayTracer.h"
#include "simpleppm.h"
#include <Eigen/Dense>
#include <memory>
#include "geometry/Sphere.h"
#include "util.h"
#include "Camera.h"
#include "materials/Phong.h"
#include "geometry/Rectangle.h"

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

            float phong_coeff = *geometry.find("pc");

            std::shared_ptr<Phong> geometry_material = std::make_shared<Phong>(ambient_color, diffuse_color, specular_color,
                                                                               ambient_coeff, diffuse_coeff, specular_coeff,
                                                                               phong_coeff);

            // Geometry type specific stuff
            std::string geometry_type = *geometry.find("type");
            if (geometry_type == "sphere") {
                std::vector<float> centre = *geometry.find("centre");
                Eigen::Vector3f centre_vector = Eigen::Vector3f(centre.at(0), centre.at(1), centre.at(2));

                float sphere_radius = *geometry.find("radius");

                world.add(std::make_shared<Sphere>(centre_vector, sphere_radius, geometry_material));
            } else if (geometry_type == "rectangle") {
                std::vector<float> p1 = *geometry.find("p1");
                Eigen::Vector3f point_1 = Eigen::Vector3f(p1.at(0), p1.at(1), p1.at(2));

                std::vector<float> p2 = *geometry.find("p2");
                Eigen::Vector3f point_2 = Eigen::Vector3f(p2.at(0), p2.at(1), p2.at(2));

                std::vector<float> p3 = *geometry.find("p3");
                Eigen::Vector3f point_3 = Eigen::Vector3f(p3.at(0), p3.at(1), p3.at(2));

                std::vector<float> p4 = *geometry.find("p4");
                Eigen::Vector3f point_4 = Eigen::Vector3f(p4.at(0), p4.at(1), p4.at(2));

                world.add(std::make_shared<Rectangle>(point_1, point_2, point_3, point_4, geometry_material));
            }
        }
    }

    if ((it = j.find("light")) != j.end()) {
        for (auto& [key, light] : (*it).items()) {
            std::string light_type = *light.find("type");
            if (light_type == "point") {
                std::vector<float> centre = *light.find("centre");
                Eigen::Vector3f light_centre = Eigen::Vector3f(centre.at(0), centre.at(1), centre.at(2));

                std::vector<float> id = *light.find("id");
                Eigen::Vector3f light_diffuse_color = Eigen::Vector3f(id.at(0), id.at(1), id.at(2));

                std::vector<float> is = *light.find("is");
                Eigen::Vector3f light_ambient_color = Eigen::Vector3f(is.at(0), is.at(1), is.at(2));

                PointLight* point_light_ptr = new PointLight(light_centre, light_diffuse_color, light_ambient_color);
                lights.push_back(point_light_ptr);
            }
        }
    }

    if ((it = j.find("output")) != j.end()) {
        for (auto& [key, output] : (*it).items()) {
            double fov = *output.find("fov");
            std::vector<double> size = *output.find("size");

            std::vector<double> lookat = *output.find("lookat");
            Eigen::Vector3f lookat_vec(lookat.at(0), lookat.at(1), lookat.at(2));

            std::vector<double> up = *output.find("up");
            Eigen::Vector3f up_vec(up.at(0), up.at(1), up.at(2));

            std::vector<double> centre = *output.find("centre");
            Eigen::Vector3f centre_vec(centre.at(0), centre.at(1), centre.at(2));

            std::vector<double> ai = *output.find("ai");
            Eigen::Vector3f ai_vec(ai.at(0), ai.at(1), ai.at(2));

            std::vector<double> bkc = *output.find("bkc");
            Eigen::Vector3f bkc_vec(bkc.at(0), bkc.at(1), bkc.at(2));

            cameras.push_back(Camera(fov, size.at(0), size.at(1), lookat_vec, up_vec, centre_vec, ai_vec, bkc_vec));
        }
    }
}

void RayTracer::run() {
    int render_number = 1;
    for (Camera camera : cameras)  {
        // Image
        const int samples_per_pixel = 20; // turn this up for good renders
        const double pp_scale = 1.0 / samples_per_pixel;
        const double max_depth = 20;

        // Render
        std::vector<double> buffer(3*camera.imageWidth*camera.imageHeight);
        for (int j = 0 ; j < camera.imageHeight ; j++) {
            std::cerr << "\rScanlines remaining: " << camera.imageHeight - j << std::flush;
            for (int i = 0 ; i < camera.imageWidth ; i++){
                Eigen::Vector3f pixel_color(0, 0, 0);

                for (int s = 1 ; s <= samples_per_pixel ; s++) {
                    double u = double(i + random_double())  / (camera.imageWidth-1);
                    double v = double(j + random_double())  / (camera.imageHeight-1);
                    Ray ray = camera.getRay(u, v);
                    pixel_color += rayColor(ray, max_depth, camera);
                }

                // scale and gamma correct and clamp
                const double r = clamp(sqrt(pixel_color.x() * pp_scale), 0.0, 0.999);
                const double g = clamp(sqrt(pixel_color.y() * pp_scale), 0.0, 0.999);
                const double b = clamp(sqrt(pixel_color.z() * pp_scale), 0.0, 0.999);

                const int row = 3 * (camera.imageHeight - j - 1) * camera.imageWidth;
                const int col = 3 * i;
                const int cell = row + col;
                buffer[cell + 0] = r;
                buffer[cell + 1] = g;
                buffer[cell + 2] = b;
            }
        }
        std::cerr << "\nDone.\n";

        std::cerr << "\nCleaning Up...\n";
        for (auto light : lights) {
            delete light;
        }


        save_ppm(std::to_string(render_number++) + ".ppm", buffer, camera.imageWidth, camera.imageHeight);
    }
}

Eigen::Vector3f RayTracer::rayColor(const Ray& ray, int depth, const Camera& camera) {
    if (depth <= 0)
        return Eigen::Vector3f(0, 0, 0);

    HitRecord hitRecord;
    if (world.hit(ray, 0.001, infinity, hitRecord)) {
        return hitRecord.material->color(ray, hitRecord, lights);

        /*
        Ray scattered;
        Eigen::Vector3f attenuation;
        if (hitRecord.material->scatter(ray, hitRecord, attenuation, scattered)) {
            return vector_multiply(attenuation, rayColor(scattered, depth-1));
        }

        return Eigen::Vector3f(0, 0, 0);
        */
    }

    return camera.bkc;
}