//
// Created by adrien on 28/02/23.
//

#include <iostream>
#include "Camera.h"
#include "RaySamplingStrategy.h"
#include "RandomRaySamplingStrategy.h"
#include "StratifiedNNRaySamplingStrategy.h"
#include "StratifiedNMRaySamplingStrategy.h"

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

std::vector<Ray> Camera::sampleRays(double pixel_bottom_left_x, double pixel_bottom_left_y) {
    std::vector<RayCoord> rayCoords = std::move(raySamplingStrategy->sampleRayCoords(pixel_bottom_left_x, pixel_bottom_left_y, raysPerPixel));

    std::vector<Ray> rays;
    rays.reserve(rayCoords.size());

    for (RayCoord rayCoord : rayCoords)
        rays.push_back(getRay(rayCoord.x, rayCoord.y));

    return rays;
}
