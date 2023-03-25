//
// Created by adrien on 10/03/23.
//

#include <iostream>
#include "Phong.h"
#include "util.h"

Phong::Phong(const Eigen::Vector3f &ambientColor, const Eigen::Vector3f &diffuseColor,
                   const Eigen::Vector3f &specularColor, float ambientCoeff, float diffuseCoeff, float specularCoeff, float phongCoeff) : Material(ambientColor, diffuseColor, specularColor, ambientCoeff, diffuseCoeff, specularCoeff, phongCoeff) {
}

Eigen::Vector3f Phong::color(const Ray& rayIn, const HitRecord& hitRecord, const std::vector<PointLight>& pointLights, const std::vector<AreaLight>& areaLights, const HittableList& world) const {
    Eigen::Vector3f ret(0, 0, 0);

    for (PointLight point_light : pointLights) {
        Eigen::Vector3f light_direction = point_light.getDirection(hitRecord);

        // Shadow ray
        HitRecord shadowHitRecord;
        if (world.hit(Ray(hitRecord.point + (shadowAcneBias * hitRecord.outwardNormal), light_direction), 0.001, (hitRecord.point + (shadowAcneBias * hitRecord.normal) - point_light.getPosition()).norm(), shadowHitRecord))
            continue;

        // Shading
        Eigen::Vector3f view_direction = -rayIn.getDirection().normalized();
        Eigen::Vector3f halfway_vector = (light_direction + view_direction).normalized();

        Eigen::Vector3f ambient = ambientCoeff * ambientColor;
        Eigen::Vector3f diffuse = vector_multiply(point_light.diffuseColor, diffuseCoeff * light_direction.dot(hitRecord.outwardNormal) * diffuseColor);
        Eigen::Vector3f specular = vector_multiply(point_light.specularColor,  specularCoeff * pow(std::max(hitRecord.outwardNormal.dot(halfway_vector), 0.0f), phongCoeff) * specularColor);

        ret += ambient + diffuse + specular;
    }

    // todo: usecenter compatability
    for (AreaLight area_light : areaLights) {
        Eigen::Vector3f temp(0, 0, 0);

        for (int area_light_row = 1 ; area_light_row <= area_light.n ; area_light_row++) {
            for (int area_light_col = 1 ; area_light_col <= area_light.n ; area_light_col++) {
                Eigen::Vector3f light_direction = area_light.getDirection(hitRecord, area_light_row, area_light_col);

                // Shadow ray
                HitRecord shadowHitRecord;
                if (world.hit(Ray(hitRecord.point + (shadowAcneBias * hitRecord.outwardNormal), light_direction), 0.001, (hitRecord.point + (shadowAcneBias * hitRecord.normal) - area_light.getPosition()).norm(), shadowHitRecord))
                    continue;

                // Shading
                Eigen::Vector3f view_direction = -rayIn.getDirection().normalized();
                Eigen::Vector3f halfway_vector = (light_direction + view_direction).normalized();

                Eigen::Vector3f ambient = ambientCoeff * ambientColor;
                Eigen::Vector3f diffuse = vector_multiply(area_light.diffuseColor, diffuseCoeff * light_direction.dot(hitRecord.outwardNormal) * diffuseColor);
                Eigen::Vector3f specular = vector_multiply(area_light.specularColor,  specularCoeff * pow(std::max(hitRecord.outwardNormal.dot(halfway_vector), 0.0f), phongCoeff) * specularColor);

                temp += ambient + diffuse + specular;
            }
        }

        int avg_factor = area_light.n * area_light.n;
        ret += Eigen::Vector3f(temp.x() / avg_factor, temp.y() / avg_factor , temp.z() / avg_factor);
    }

    return ret;
}
