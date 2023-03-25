//
// Created by adrien on 10/03/23.
//

#include <iostream>
#include "Phong.h"
#include "util.h"

Phong::Phong(const Eigen::Vector3f &ambientColor, const Eigen::Vector3f &diffuseColor,
                   const Eigen::Vector3f &specularColor, float ambientCoeff, float diffuseCoeff, float specularCoeff, float phongCoeff) : Material(ambientColor, diffuseColor, specularColor, ambientCoeff, diffuseCoeff, specularCoeff, phongCoeff) {
}

Eigen::Vector3f Phong::color(const Ray& rayIn, const HitRecord& hitRecord, const std::vector<PointLight>& pointLights, const std::vector<AreaLight>& areaLights, const HittableList& world, bool globalIllumination, bool antiAliasing) const {
    Eigen::Vector3f ret(0, 0, 0);

    for (PointLight point_light : pointLights) {
        ret += phongShade(rayIn, hitRecord, world, point_light.getPosition(), point_light.getDirection(hitRecord), point_light.diffuseColor, point_light.specularColor, globalIllumination);
    }

    for (AreaLight area_light : areaLights) {
        if (area_light.useCenter || antiAliasing) {
            ret += phongShade(rayIn, hitRecord, world, area_light.getPosition(), area_light.getDirection(hitRecord), area_light.diffuseColor, area_light.specularColor, globalIllumination);
        } else {
            Eigen::Vector3f temp(0, 0, 0);

            for (int area_light_row = 1 ; area_light_row <= area_light.n ; area_light_row++) {
                for (int area_light_col = 1 ; area_light_col <= area_light.n ; area_light_col++) {
                    Eigen::Vector3f light_direction = area_light.getDirection(hitRecord, area_light_row, area_light_col);

                    temp += phongShade(rayIn, hitRecord, world, area_light.getPosition(), area_light.getDirection(hitRecord, area_light_row, area_light_col), area_light.diffuseColor, area_light.specularColor, globalIllumination);
                }
            }

            int avg_factor = area_light.n * area_light.n;
            ret += Eigen::Vector3f(temp.x() / avg_factor, temp.y() / avg_factor , temp.z() / avg_factor);
        }
    }

    return ret;
}

Eigen::Vector3f Phong::phongShade(const Ray& rayIn, const HitRecord& hitRecord, const HittableList& world, const Eigen::Vector3f light_position, const Eigen::Vector3f light_direction, const Eigen::Vector3f& diffuse_color, const Eigen::Vector3f& specular_color, bool globalIllumination) const {
    // Shadow ray
    HitRecord shadowHitRecord;
    if (world.hit(Ray(hitRecord.point + (shadowAcneBias * hitRecord.outwardNormal), light_direction), 0.001, (hitRecord.point + (shadowAcneBias * hitRecord.normal) - light_position).norm(), shadowHitRecord))
        return Eigen::Vector3f(0, 0, 0);

    // Shading
    Eigen::Vector3f diffuse = vector_multiply(diffuse_color, diffuseCoeff * light_direction.dot(hitRecord.outwardNormal) * diffuseColor);

    if (globalIllumination)
        return diffuse;

    Eigen::Vector3f view_direction = -rayIn.getDirection().normalized();
    Eigen::Vector3f halfway_vector = (light_direction + view_direction).normalized();

    Eigen::Vector3f ambient = ambientCoeff * ambientColor;
    Eigen::Vector3f specular = vector_multiply(specular_color, specularCoeff * pow(std::max(hitRecord.outwardNormal.dot(halfway_vector), 0.0f), phongCoeff) * specularColor);

    return ambient + diffuse + specular;
}

Ray Phong::scatter(const Ray& rayIn, const HitRecord& hitRecord, bool twoSideRender) const {
    Eigen::Vector3f reflection_normal = twoSideRender ? hitRecord.outwardNormal : hitRecord.normal;
    Eigen::Vector3f reflection_direction = reflect_vector(rayIn.getDirection().normalized(), reflection_normal);
    Eigen::Vector3f scatter_direction = reflection_direction + random_in_hemisphere(reflection_normal);

    // Catch degenerate scatter direction
    if (scatter_direction.isZero())
        scatter_direction = reflection_normal;

    return Ray(hitRecord.point, scatter_direction);
}
