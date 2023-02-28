//
// Created by adrien on 28/02/23.
//

#include "Metal.h"
#include "util.h"

Metal::Metal(const Eigen::Vector3f albedo) : albedo(albedo) {

}

bool Metal::scatter(const Ray &rayIn, const HitRecord &hitRecord, Eigen::Vector3f attenuation, Ray &scattered) const {
    Eigen::Vector3f reflected = reflect_vector(rayIn.getDirection().normalized(), hitRecord.normal);
    scattered = Ray(hitRecord.point, reflected);
    attenuation = albedo;
    return scattered.getDirection().dot(hitRecord.normal) > 0;
}
