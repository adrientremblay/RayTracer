//
// Created by adrien on 28/02/23.
//

#include "Lambertian.h"

Lambertian::Lambertian(const Eigen::Vector3f albedo) : albedo(albedo){

}

bool Lambertian::scatter(const Ray& rayIn, const HitRecord& hitRecord, Eigen::Vector3f attenuation, Ray& scattered) const {
    Eigen::Vector3f scatter_direction = hitRecord.normal + random_in_hemisphere(hitRecord.normal);

    if (scatter_direction.isZero())
        scatter_direction = hitRecord.normal;

    scattered = Ray(hitRecord.point, scatter_direction);
    attenuation = albedo;
    return true;
}