//
// Created by adrien on 10/03/23.
//

#include <iostream>
#include "Phong.h"

Phong::Phong(const Eigen::Vector3f &ambientColor, const Eigen::Vector3f &diffuseColor,
                   const Eigen::Vector3f &specularColor, float ambientCoeff, float diffuseCoeff, float specularCoeff, float phongCoeff) : Material(ambientColor, diffuseColor, specularColor, ambientCoeff, diffuseCoeff, specularCoeff, phongCoeff) {
}

Eigen::Vector3f Phong::color(const Ray& rayIn, const HitRecord& hitRecord, const std::vector<Light*>& lights, const HittableList& world) const {
    Eigen::Vector3f ret(0, 0, 0);

    for (Light* light_ptr : lights) {
        Eigen::Vector3f light_direction = light_ptr->getDirection(hitRecord);

        // Shadow ray
        HitRecord shadowHitRecord;
        if (world.hit(Ray(hitRecord.point + (shadowAcneBias * hitRecord.normal), light_direction), 0.001, infinity, shadowHitRecord))  {
            continue;
        }

        // Shading
        Eigen::Vector3f view_direction = -rayIn.getDirection().normalized();
        Eigen::Vector3f halfway_vector = (light_direction + view_direction).normalized();

        Eigen::Vector3f ambient = vector_multiply(light_ptr->specularColor, ambientCoeff * ambientColor);
        Eigen::Vector3f diffuse = vector_multiply(light_ptr->diffuseColor, diffuseCoeff * light_direction.dot(hitRecord.normal) * diffuseColor);
        Eigen::Vector3f specular = specularCoeff * pow(std::max(hitRecord.normal.dot(halfway_vector), 0.0f), phongCoeff) * specularColor;
        //Eigen::Vector3f specular = specularCoeff * std::pow((reflect_vector(-light_direction, hitRecord.normal)).dot(view_direction), phongCoeff) * specularColor;

        ret += ambient + diffuse + specular;
    }

    return ret;
}
