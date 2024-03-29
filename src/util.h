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
 * @param max
 * @return random double in [0, max)
 */
inline double random_double(double max) {
    return rand() / (RAND_MAX + max);
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

inline Eigen::Vector3f random_in_hemisphere(const Eigen::Vector3f& normal) {
    Eigen::Vector3f rando = random_in_unit_sphere();
    if (rando.dot(normal) > 0.0)
        return rando;
    else
        return -rando;
}

inline Eigen::Vector3f reflect_vector(const Eigen::Vector3f& vector, const Eigen::Vector3f& normal) {
    return vector - 2 * vector.dot(normal) * normal;
}

inline Eigen::Vector3f vector_multiply(const Eigen::Vector3f& v1, const Eigen::Vector3f& v2) {
    float x = v1.x() * v2.x();
    float y = v1.y() * v2.y();
    float z = v1.z() * v2.z();
    return Eigen::Vector3f(x, y, z);
}

inline Eigen::Vector3f refract(const Eigen::Vector3f& ray, const Eigen::Vector3f& normal, double etai_over_etat) {
    auto cos_theta = fmin((-ray).dot(normal), 1);
    Eigen::Vector3f ray_out_perp = etai_over_etat * (ray + cos_theta * normal);
    Eigen::Vector3f ray_out_parallel = -sqrt(fabs(1.0 - ray_out_perp.squaredNorm())) * normal;
    return ray_out_perp + ray_out_parallel;
}

#endif //RAYTRACER_UTIL_H
