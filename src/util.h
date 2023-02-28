//
// Created by adrien on 28/02/23.
//

#ifndef RAYTRACER_UTIL_H
#define RAYTRACER_UTIL_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include <Eigen/Dense>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degreesToRadians(double degrees) {
    return degrees * pi / 180.0;
}

/**
 * @return random double in [0, 1)
 */
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

/**
 *
 * @param min
 * @param max
 * @return random double in [min, max)
 */
inline double random_double(double min, double max) {
   return min + (max-min) * random_double();
}

inline double clamp(double x, double min, double max) {
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

inline Eigen::Vector3f random_vector() {
    return Eigen::Vector3f(random_double(), random_double(), random_double());
}

inline Eigen::Vector3f random_vector(double min, double max) {
    return Eigen::Vector3f(random_double(min, max), random_double(min, max), random_double(min, max));
}

inline Eigen::Vector3f random_in_unit_sphere() {
    while (true) {
        Eigen::Vector3f point = random_vector(-1, 1);
        if (point.squaredNorm() >= 1)
            continue;
        return point;
    }
}

inline Eigen::Vector3f random_in_unit_sphere_normalized() {
    return random_in_unit_sphere().normalized();
}

#endif //RAYTRACER_UTIL_H
