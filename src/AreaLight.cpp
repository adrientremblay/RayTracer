//
// Created by adrien on 13/03/23.
//

#include "AreaLight.h"
#include "util.h"

AreaLight::AreaLight(Eigen::Vector3f diffuseColor, Eigen::Vector3f specularColor, Eigen::Vector3f p1, Eigen::Vector3f p2, Eigen::Vector3f p3, Eigen::Vector3f p4,
bool useCenter, double n) : Light(diffuseColor, specularColor), p1(p1), p2(p2), p3(p3), p4(p4), useCenter(useCenter), n(n) {
    Eigen::Vector3f v1 = p2 - p1;
    Eigen::Vector3f v2 = p3 - p1;
    normal = v1.cross(v2).normalized();

    p1p2 = (p2 - p1);
    p2p3 = (p3 - p2);
    center = p1 + (0.5*p1p2) + (0.5*p2p3);

    double cell_len = 1 / n;
    half_cell_len = cell_len / 2;
}

Eigen::Vector3f AreaLight::getDirection(const HitRecord& hitRecord) const {
    return (center - hitRecord.point).normalized();
}

Eigen::Vector3f AreaLight::getDirection(const HitRecord& hitRecord, int area_light_row, int area_light_col) const {
    float s1 = double((area_light_row/n) - half_cell_len);
    float s2 = double((area_light_col/n) - half_cell_len);

    Eigen::Vector3f cell_center = p1 + (s1 * p1p2) + (s2 * p2p3);
    return (cell_center - hitRecord.point).normalized();
}

Eigen::Vector3f AreaLight::getPosition() const {
    return center;
}
