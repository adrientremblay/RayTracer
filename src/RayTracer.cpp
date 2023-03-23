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
#include "Phong.h"
#include "Rectangle.h"
#include "AreaLight.h"

RayTracer::RayTracer(nlohmann::json& j) {
    // Parsing geometry section
    nlohmann::json::iterator it;
    if ((it = j.find("geometry")) != j.end()) {
        for (auto& geometry: (*it).items()) {
            // Material
            std::vector<float> ac = *geometry.value().find("ac");
            Eigen::Vector3f ambient_color = Eigen::Vector3f(ac.at(0), ac.at(1), ac.at(2));

            std::vector<float> dc = *geometry.value().find("dc");
            Eigen::Vector3f diffuse_color = Eigen::Vector3f(dc.at(0), dc.at(1), dc.at(2));

            std::vector<float> sc = *geometry.value().find("sc");
            Eigen::Vector3f specular_color = Eigen::Vector3f(sc.at(0), sc.at(1), sc.at(2));

            float ambient_coeff = *geometry.value().find("ka");
            float diffuse_coeff = *geometry.value().find("kd");
            float specular_coeff = *geometry.value().find("ks");

            float phong_coeff = *geometry.value().find("pc");

            std::shared_ptr<Phong> geometry_material = std::make_shared<Phong>(ambient_color, diffuse_color, specular_color,
                                                                               ambient_coeff, diffuse_coeff, specular_coeff,
                                                                               phong_coeff);

            // Geometry type specific stuff
            std::string geometry_type = *geometry.value().find("type");
            if (geometry_type == "sphere") {
                std::vector<float> centre = *geometry.value().find("centre");
                Eigen::Vector3f centre_vector = Eigen::Vector3f(centre.at(0), centre.at(1), centre.at(2));

                float sphere_radius = *geometry.value().find("radius");

                world.add(std::make_shared<Sphere>(centre_vector, sphere_radius, geometry_material));
            } else if (geometry_type == "rectangle") {
                std::vector<float> p1 = *geometry.value().find("p1");
                Eigen::Vector3f point_1 = Eigen::Vector3f(p1.at(0), p1.at(1), p1.at(2));

                std::vector<float> p2 = *geometry.value().find("p2");
                Eigen::Vector3f point_2 = Eigen::Vector3f(p2.at(0), p2.at(1), p2.at(2));

                std::vector<float> p3 = *geometry.value().find("p3");
                Eigen::Vector3f point_3 = Eigen::Vector3f(p3.at(0), p3.at(1), p3.at(2));

                std::vector<float> p4 = *geometry.value().find("p4");
                Eigen::Vector3f point_4 = Eigen::Vector3f(p4.at(0), p4.at(1), p4.at(2));

                world.add(std::make_shared<Rectangle>(point_1, point_2, point_3, point_4, geometry_material));
            }
        }
    }

    if ((it = j.find("light")) != j.end()) {
        for (auto& light: (*it).items()) {
            std::vector<float> id = *light.value().find("id");
            Eigen::Vector3f light_diffuse_color = Eigen::Vector3f(id.at(0), id.at(1), id.at(2));

            std::vector<float> is = *light.value().find("is");
            Eigen::Vector3f light_specular_color = Eigen::Vector3f(is.at(0), is.at(1), is.at(2));

            std::string light_type = *light.value().find("type");
            if (light_type == "point") {
                std::vector<float> centre = *light.value().find("centre");
                Eigen::Vector3f light_centre = Eigen::Vector3f(centre.at(0), centre.at(1), centre.at(2));

                PointLight* point_light_ptr = new PointLight(light_centre, light_diffuse_color, light_specular_color);
                lights.push_back(point_light_ptr);
            } else if (light_type == "area") {
                std::vector<float> p1 = *light.value().find("p1");
                Eigen::Vector3f point_1 = Eigen::Vector3f(p1.at(0), p1.at(1), p1.at(2));

                std::vector<float> p2 = *light.value().find("p2");
                Eigen::Vector3f point_2 = Eigen::Vector3f(p2.at(0), p2.at(1), p2.at(2));

                std::vector<float> p3 = *light.value().find("p3");
                Eigen::Vector3f point_3 = Eigen::Vector3f(p3.at(0), p3.at(1), p3.at(2));

                std::vector<float> p4 = *light.value().find("p4");
                Eigen::Vector3f point_4 = Eigen::Vector3f(p4.at(0), p4.at(1), p4.at(2));

                bool use_center = *light.value().find("usecenter");

                AreaLight* area_light_ptr = new AreaLight(light_diffuse_color, light_specular_color, point_1, point_2, point_3, point_4, use_center);
                lights.push_back(area_light_ptr);
            }
        }
    }

    if ((it = j.find("output")) != j.end()) {
        for (auto& output : (*it).items()) {
            double fov = *output.value().find("fov");
            std::vector<double> size = *output.value().find("size");

            std::vector<double> lookat = *output.value().find("lookat");
            Eigen::Vector3f lookat_vec(lookat.at(0), lookat.at(1), lookat.at(2));

            std::vector<double> up = *output.value().find("up");
            Eigen::Vector3f up_vec(up.at(0), up.at(1), up.at(2));

            std::vector<double> centre = *output.value().find("centre");
            Eigen::Vector3f centre_vec(centre.at(0), centre.at(1), centre.at(2));

            std::vector<double> ai = *output.value().find("ai");
            Eigen::Vector3f ai_vec(ai.at(0), ai.at(1), ai.at(2));

            std::vector<double> bkc = *output.value().find("bkc");
            Eigen::Vector3f bkc_vec(bkc.at(0), bkc.at(1), bkc.at(2));

            std::string filename = *output.value().find("filename");

            // Optional output parameters
            bool globalIllumination = false;
            nlohmann::json::iterator globalIllumIter = output.value().find("globalillum");
            if (globalIllumIter != output.value().end()) {
                globalIllumination = *globalIllumIter;
            }

            std::vector<int> raysPerPixel = {1};
            nlohmann::json::iterator raysPerPixelIter = output.value().find("raysperpixel");
            if (raysPerPixelIter != output.value().end()) {
                raysPerPixel = raysPerPixelIter->get<std::vector<int>>();
            }

            int maxBounces = 1;
            nlohmann::json::iterator maxBouncesIter = output.value().find("maxbounces");
            if (maxBouncesIter != output.value().end()) {
                maxBounces = *maxBouncesIter;
            }

            float probTerminate = 1;
            nlohmann::json::iterator probTerminateIter = output.value().find("probterminate");
            if (probTerminateIter != output.value().end()) {
                probTerminate = *probTerminateIter;
            }

            cameras.push_back(Camera(fov, size.at(0), size.at(1), lookat_vec, up_vec, centre_vec, ai_vec, bkc_vec, filename, globalIllumination, raysPerPixel, maxBounces, probTerminate));
        }
    }
}

void RayTracer::run() {
    for (Camera camera : cameras)  {
        // Image
        const double pp_scale = 1.0 / camera.raysPerPixel.at(0);

        // Render
        std::vector<double> buffer(3*camera.imageWidth*camera.imageHeight);
        for (int j = 0 ; j < camera.imageHeight ; j++) {
            std::cerr << "\rScanlines remaining: " << camera.imageHeight - j << std::flush;
            for (int i = 0 ; i < camera.imageWidth ; i++){
                Eigen::Vector3f pixel_color(0, 0, 0);

                for (int s = 1 ; s <= camera.raysPerPixel.at(0) ; s++) {
                    double u = double(i + random_double())  / (camera.imageWidth-1);
                    double v = double(j + random_double())  / (camera.imageHeight-1);
                    Ray ray = camera.getRay(u, v);
                    pixel_color += rayColor(ray, camera.maxBounces, camera);
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

        save_ppm(camera.filename, buffer, camera.imageWidth, camera.imageHeight);
    }
}

Eigen::Vector3f RayTracer::rayColor(const Ray& ray, int depth, const Camera& camera) {
    if (depth <= 0)
        return Eigen::Vector3f(0, 0, 0);

    HitRecord hitRecord;
    if (world.hit(ray, 0.001, infinity, hitRecord)) {
        return hitRecord.material->color(ray, hitRecord, lights, world);

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