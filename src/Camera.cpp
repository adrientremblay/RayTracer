//
// Created by adrien on 28/02/23.
//

#include <iostream>
#include "Camera.h"
#include "Hittable.h"
#include <memory>
#include "Material.h"
#include "settings.h"

int Camera::camera_count = 1;

Camera::Camera(double fov, double imageWidth, double imageHeight, Eigen::Vector3f lookat, Eigen::Vector3f up, Eigen::Vector3f centre, Eigen::Vector3f ai, Eigen::Vector3f bkc, std::string filename, bool globalIllumination, const std::vector<int>& raysPerPixel, int maxBounces, float probTerminate, bool antiAliasing, bool twoSideRender) :
fov(fov), imageWidth(imageWidth), imageHeight(imageHeight), lookat(lookat.normalized()), up(up.normalized()), centre(centre), ai(ai), bkc(bkc), filename(filename), globalIllumination(globalIllumination), raysPerPixel(raysPerPixel), maxBounces(maxBounces), probTerminate(probTerminate), antiAliasing(antiAliasing), twoSideRender(twoSideRender) {
    double theta = degreesToRadians(fov);
    double h = tan(theta / 2);

    aspectRatio = imageWidth / imageHeight;

    auto viewport_height = 2.0 * h;
    auto viewport_width = aspectRatio * viewport_height;

    Eigen::Vector3f w = -lookat.normalized();
    Eigen::Vector3f u = (up.cross(w)).normalized();
    Eigen::Vector3f v = w.cross(u);

    horizontal = viewport_width * u;
    vertical = viewport_height * v;
    lowerLeftCorner = centre - horizontal / 2 - vertical / 2 - (focal_length * w);

    if ((antiAliasing || globalIllumination) && raysPerPixel.size() > 1)  {
        if (raysPerPixel.size() == 2) {
            raySamplingStrat = NN;
        } else {
            raySamplingStrat = NM;
        }
    } else {
        raySamplingStrat = RANDOM;
    }

    cameraNum = camera_count++;

#if PRINT_DEBUG_INFO
    std::cout
        << "\n"
        << "Camera " << cameraNum << " created with the following values:\n"
        << "\tFOV: " << fov << '\n'
        << "\tImage Width: " << imageWidth << '\n'
        << "\tImage Height: " << imageHeight << '\n'
        << "\tLookat Vector: " << lookat.transpose() << '\n'
        << "\tUp Vector: " << up.transpose() << '\n'
        << "\tCentre Vector: " << centre.transpose() << '\n'
        << "\tai: " << ai.transpose() << '\n'
        << "\tbkc: " << bkc.transpose() << '\n'
        << "\tfilename: " << filename << '\n'
        << "\tGlobal Illumination: " << globalIllumination << '\n'
        << "\traysPerPixel: "; for (int v : raysPerPixel) std::cout << v << ' ';
    std::cout
        << "\tTwo Side Render: " << twoSideRender << '\n'
        << "\tMax Bounces: " << maxBounces << '\n'
        << "\tProb Termination: " << maxBounces << '\n'
        << "\tAnti-Aliasing: " << antiAliasing << '\n'
        << "\tTwo Side Rendering: " << antiAliasing << '\n'
    ;
#endif
}

Ray Camera::getRay(double ray_x, double ray_y) const {
    return Ray(centre, (lowerLeftCorner + (ray_x / imageWidth) * horizontal + (ray_y / imageHeight) * vertical - centre).normalized(), true);
}

void Camera::sampleRays(double pixel_bottom_left_x, double pixel_bottom_left_y, Eigen::Vector3f& pixelColor, const HittableList& world, const std::vector<PointLight>& pointLights, const std::vector<AreaLight>& areaLights) {
    int successful_rays = 0;

    // todo: make the switch cleaner
    switch (raySamplingStrat) {
        int num_rays;
        int strata_cols, strata_rows, rays_per_strata;
        double strata_width, strata_height;
        case RANDOM:
            num_rays = raysPerPixel.at(0);

            for (int ray_number = 1 ; ray_number <= num_rays ; ray_number++) {
                double ray_x = pixel_bottom_left_x + random_double();
                double ray_y = pixel_bottom_left_y + random_double();

                sampleRay(ray_x, ray_y, pixelColor, world, pointLights, areaLights, successful_rays);
            }
            break;
        case NN:
            rays_per_strata = raysPerPixel.at(raysPerPixel.size()-1);
            strata_cols = strata_rows = raysPerPixel.at(0);
            strata_width = strata_height = 1 / strata_cols;

            num_rays = rays_per_strata * strata_cols * strata_rows;

            for (int strata_x = 0 ; strata_x < strata_cols ; strata_x++) {
                for (int strata_y = 0 ; strata_y < strata_cols ; strata_y++) {
                    for (int ray_number = 1 ; ray_number <= rays_per_strata ; ray_number++) {
                        double ray_x = pixel_bottom_left_x + (strata_x * strata_width) + random_double(strata_width);
                        double ray_y = pixel_bottom_left_y + (strata_y * strata_height) + random_double(strata_height);

                        sampleRay(ray_x, ray_y, pixelColor, world, pointLights, areaLights, successful_rays);
                    }
                }
            }
            break;
        case NM:
            rays_per_strata = raysPerPixel.at(raysPerPixel.size()-1);
            strata_cols = raysPerPixel.at(1);
            strata_rows = raysPerPixel.at(0);
            strata_width = 1 / strata_cols;
            strata_height = 1 / strata_rows;

            num_rays = rays_per_strata * strata_cols * strata_rows;

            for (int strata_x = 0 ; strata_x < strata_cols ; strata_x++) {
                for (int strata_y = 0 ; strata_y < strata_cols ; strata_y++) {
                    for (int ray_number = 1 ; ray_number <= rays_per_strata ; ray_number++) {
                        double ray_x = double(pixel_bottom_left_x + (strata_x * strata_width) + random_double(strata_width));
                        double ray_y = double(pixel_bottom_left_y + (strata_y * strata_height) + random_double(strata_height));

                        sampleRay(ray_x, ray_y, pixelColor, world, pointLights, areaLights, successful_rays);
                    }
                }
            }
            break;
    }

    if (successful_rays == 0) {
        pixelColor = Eigen::Vector3f(0,0,0);
        return;
    }

    pixelColor = Eigen::Vector3f(pixelColor.x() / successful_rays, pixelColor.y() / successful_rays, pixelColor.z() / successful_rays);
}

void Camera::sampleRay(double ray_x, double ray_y, Eigen::Vector3f& pixelColor, const HittableList& world, const std::vector<PointLight>& pointLights, const std::vector<AreaLight>& areaLights, int& successfulRays) {
    Ray ray = getRay(ray_x, ray_y);

    if (!globalIllumination) {
        // Ray tracing the ray
        pixelColor += rayTrace(ray, world, pointLights, areaLights);
        successfulRays++;
    } else {
        // Path tracing the ray
        bool hitNothing = false;
        Eigen::Vector3f path_trace_color = pathTrace(ray, world, pointLights, areaLights, maxBounces, hitNothing);

        if (!hitNothing) {
            successfulRays++;
            pixelColor += path_trace_color;
        }
    }
}

Eigen::Vector3f Camera::rayTrace(const Ray& ray, const HittableList& world, const std::vector<PointLight>& pointLights, const std::vector<AreaLight>& areaLights) {
    HitRecord hitRecord;
    if (world.hit(ray, 0.001, infinity, hitRecord)) {
        return hitRecord.material->color(ray, hitRecord, pointLights, areaLights, world, globalIllumination, antiAliasing);
    }

    return bkc;
}

Eigen::Vector3f Camera::pathTrace(const Ray& ray, const HittableList& world, const std::vector<PointLight>& pointLights, const std::vector<AreaLight>& areaLights, int depth, bool& hitNothing) {
    HitRecord hitRecord;
    if (world.hit(ray, 0.001, infinity, hitRecord)) {
        if (depth <= 0 || random_double() <= probTerminate) {
            // finally now we use the lights to calculate the final result
            return hitRecord.material->color(ray, hitRecord, pointLights, areaLights, world, globalIllumination, antiAliasing);
        }

        Ray scatterRay = hitRecord.material->scatter(ray, hitRecord, twoSideRender);
        return vector_multiply(hitRecord.material->diffuseColor, pathTrace(scatterRay, world, pointLights, areaLights, depth-1, hitNothing));
    }

    hitNothing = true;
    return Eigen::Vector3f(0,0,0); // should not be used
}
