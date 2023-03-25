//
// Created by adrien on 28/02/23.
//

#include <iostream>
#include "Camera.h"
#include "RaySamplingStrategy.h"
#include "RandomRaySamplingStrategy.h"
#include "StratifiedNNRaySamplingStrategy.h"
#include "StratifiedNMRaySamplingStrategy.h"
#include "Hittable.h"
#include <memory>
#include "Material.h"

#define PRINT_CAMERA_INFO

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
            raySamplingStrategy = new StratifiedNNRaySamplingStrategy();
        } else {
            raySamplingStrategy = new StratifiedNMRaySamplingStrategy();
        }
    } else {
        raySamplingStrategy = new RandomRaySamplingStrategy();
    }

#ifdef PRINT_CAMERA_INFO
    std::cout << "Camera created with the following values \n"
        << "FOV: " << fov << '\n'
        << "Image Width: " << imageWidth << '\n'
        << "Image Height: " << imageHeight << '\n'
        << "Lookat Vector: " << lookat.transpose() << '\n'
        << "Up Vector: " << up.transpose() << '\n'
        << "Centre Vector: " << centre.transpose() << '\n'
        << "ai: " << ai.transpose() << '\n'
        << "bkc: " << bkc.transpose() << '\n'
        << "filename: " << filename << '\n'
        << "Global Illumination: " << globalIllumination << '\n'
    //    << "raysPerPixel: " << raysPerPixel << '\n'
        << "Two Side Render: " << twoSideRender << '\n'
    ;
#endif
}

Ray Camera::getRay(double ray_x, double ray_y) const {
    return Ray(centre, (lowerLeftCorner + (ray_x / imageWidth) * horizontal + (ray_y / imageHeight) * vertical - centre).normalized(), true);
}

void Camera::sampleRays(double pixel_bottom_left_x, double pixel_bottom_left_y, Eigen::Vector3f& pixelColor, const HittableList& world, const std::vector<PointLight>& pointLights, const std::vector<AreaLight>& areaLights) {
    // Getting the ray coordinates todo: remove this
    std::vector<RayCoord> rayCoords = std::move(raySamplingStrategy->sampleRayCoords(pixel_bottom_left_x, pixel_bottom_left_y, raysPerPixel));

    int successful_rays = rayCoords.size();
    for (RayCoord rayCoord : rayCoords) {
        Ray ray = getRay(rayCoord.x, rayCoord.y);

        if (!globalIllumination) {
            // Ray tracing the ray
            pixelColor += rayTrace(ray, world, pointLights, areaLights);
        } else {
            // Path tracing the ray
            bool hitNothing = false;
            Eigen::Vector3f path_trace_color = pathTrace(ray, world, pointLights, areaLights, maxBounces, hitNothing);

            if (hitNothing) {
                successful_rays--;
            } else {
                pixelColor += path_trace_color;
            }
        }
    }

    pixelColor = Eigen::Vector3f(pixelColor.x() / successful_rays, pixelColor.y() / successful_rays, pixelColor.z() / successful_rays);
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
