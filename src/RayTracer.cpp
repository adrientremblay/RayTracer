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
#include <thread>
#include "settings.h"

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

                pointLights.push_back(PointLight(light_centre, light_diffuse_color, light_specular_color));
            } else if (light_type == "area") {
                std::vector<float> p1 = *light.value().find("p1");
                Eigen::Vector3f point_1 = Eigen::Vector3f(p1.at(0), p1.at(1), p1.at(2));

                std::vector<float> p2 = *light.value().find("p2");
                Eigen::Vector3f point_2 = Eigen::Vector3f(p2.at(0), p2.at(1), p2.at(2));

                std::vector<float> p3 = *light.value().find("p3");
                Eigen::Vector3f point_3 = Eigen::Vector3f(p3.at(0), p3.at(1), p3.at(2));

                std::vector<float> p4 = *light.value().find("p4");
                Eigen::Vector3f point_4 = Eigen::Vector3f(p4.at(0), p4.at(1), p4.at(2));

                bool use_center = false;
                nlohmann::json::iterator userCenterIter = light.value().find("usecenter");
                if (userCenterIter != light.value().end())
                    use_center = *userCenterIter;

                double n = 10.0f;
                nlohmann::json::iterator nIter = light.value().find("n");
                if (nIter != light.value().end())
                    n = nIter->get<double>();

                areaLights.push_back(AreaLight(light_diffuse_color, light_specular_color, point_1, point_2, point_3, point_4, use_center, n));
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

            int maxBounces = 1;
            nlohmann::json::iterator maxBouncesIter = output.value().find("maxbounces");
            if (maxBouncesIter != output.value().end()) {
                maxBounces = *maxBouncesIter;
            }

            float probTerminate = 0.333f;
            nlohmann::json::iterator probTerminateIter = output.value().find("probterminate");
            if (probTerminateIter != output.value().end()) {
                probTerminate = *probTerminateIter;
            }

            bool antiAliasing = false;
            nlohmann::json::iterator antiAliasingIter = output.value().find("antialiasing");
            if (!globalIllumination && antiAliasingIter != output.value().end()) {
                antiAliasing = *antiAliasingIter;
            }

            std::vector<int> raysPerPixel = {1};
            nlohmann::json::iterator raysPerPixelIter = output.value().find("raysperpixel");
            if ((antiAliasing || globalIllumination) && raysPerPixelIter != output.value().end()) {
                raysPerPixel = raysPerPixelIter->get<std::vector<int>>();
            }

            bool twoSideRender = true;
            nlohmann::json::iterator twoSideRenderIter = output.value().find("twosiderender");
            if (twoSideRenderIter != output.value().end()) {
                twoSideRender = *twoSideRenderIter;
            }

            cameras.push_back(Camera(fov, size.at(0), size.at(1), lookat_vec, up_vec, centre_vec, ai_vec, bkc_vec, filename, globalIllumination, raysPerPixel, maxBounces, probTerminate, antiAliasing, twoSideRender));
        }
    }
}

void RayTracer::run() {
    for (Camera camera : cameras)  {
#if PRINT_DEBUG_INFO
        std::cout << "\nStarting render for camera " << camera.cameraNum << "..." << std::endl;
#endif
        std::vector<double> buffer(3*camera.imageWidth*camera.imageHeight);
        double rows_rendered = 0;

#if LEVERAGE_MULTITHREADING
        const int num_threads = 8;
        const int rows_per_thread = camera.imageHeight / num_threads; // todo: what if this isn't a nice int???
        std::vector<std::thread> threads(num_threads);

        for (int thread_index = 0 ; thread_index < num_threads ; thread_index++) {
            threads[thread_index] = std::thread([&, thread_index]() {
                for (int pixel_bottom_left_y = (thread_index * rows_per_thread) ; pixel_bottom_left_y < ((thread_index + 1) * rows_per_thread) ; pixel_bottom_left_y++) {
                    for (int pixel_bottom_left_x = 0 ; pixel_bottom_left_x < camera.imageWidth ; pixel_bottom_left_x++) {
                        Eigen::Vector3f pixel_color(0, 0, 0);

                        camera.sampleRays(pixel_bottom_left_x, pixel_bottom_left_y, pixel_color, world, pointLights, areaLights);

                        // scale, gamma correct and clamp
                        const double r = clamp(gammaCorrect(pixel_color.x()), 0.0, 0.999);
                        const double g = clamp(gammaCorrect(pixel_color.y()), 0.0, 0.999);
                        const double b = clamp(gammaCorrect(pixel_color.z()), 0.0, 0.999);

                        const int row = 3 * (camera.imageHeight - pixel_bottom_left_y - 1) * camera.imageWidth;
                        const int col = 3 * pixel_bottom_left_x;
                        const int cell = row + col;
                        buffer[cell + 0] = r;
                        buffer[cell + 1] = g;
                        buffer[cell + 2] = b;
                    }
                    rows_rendered++;
#if PRINT_DEBUG_INFO
                    std::cout << "Rendering is " << (rows_rendered/camera.imageHeight)*100 << "% complete\n";
#endif
                }
            });
        }

        // Wait for all threads to complete
        for (int thread_index = 0 ; thread_index < num_threads ; thread_index++) {
            threads[thread_index].join();
        }
#else
        for (int pixel_bottom_left_y = 0 ; pixel_bottom_left_y < camera.imageHeight ; pixel_bottom_left_y++) {
            for (int pixel_bottom_left_x = 0 ; pixel_bottom_left_x < camera.imageWidth ; pixel_bottom_left_x++) {
                Eigen::Vector3f pixel_color(0, 0, 0);

                camera.sampleRays(pixel_bottom_left_x, pixel_bottom_left_y, pixel_color, world, pointLights, areaLights);

                // scale, gamma correct and clamp
                const double r = clamp(gammaCorrect(pixel_color.x()), 0.0, 0.999);
                const double g = clamp(gammaCorrect(pixel_color.y()), 0.0, 0.999);
                const double b = clamp(gammaCorrect(pixel_color.z()), 0.0, 0.999);

                const int row = 3 * (camera.imageHeight - pixel_bottom_left_y - 1) * camera.imageWidth;
                const int col = 3 * pixel_bottom_left_x;
                const int cell = row + col;
                buffer[cell + 0] = r;
                buffer[cell + 1] = g;
                buffer[cell + 2] = b;
            }
            rows_rendered++;
#if PRINT_DEBUG_INFO
            std::cout << "Rendering is " << (rows_rendered/camera.imageHeight)*100 << "% complete\n";
#endif
        }
#endif

#if PRINT_DEBUG_INFO
        std::cout << "\nDone all renders\n";
#endif

        save_ppm(camera.filename, buffer, camera.imageWidth, camera.imageHeight);
    }
}


inline float RayTracer::gammaCorrect(float color) {
# if GAMMA_CORRECTION_ENABLED
    return sqrt(color);
# else
    return color;
# endif
}