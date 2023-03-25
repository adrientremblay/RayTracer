//
// Created by adrien on 13/03/23.
//

#ifndef RAYTRACER_AREALIGHT_H
#define RAYTRACER_AREALIGHT_H

#include "Light.h"

class AreaLight : public Light {
public:
    Eigen::Vector3f p1;
    Eigen::Vector3f p2;
    Eigen::Vector3f p3;
    Eigen::Vector3f p4;
    bool useCenter;
    Eigen::Vector3f normal;
    Eigen::Vector3f center;
    double n;

    AreaLight(Eigen::Vector3f diffuseColor, Eigen::Vector3f specularColor, Eigen::Vector3f p1, Eigen::Vector3f p2, Eigen::Vector3f p3, Eigen::Vector3f p4,
              bool useCenter, double n);

    virtual Eigen::Vector3f getDirection(const HitRecord& hitRecord) const override;
    virtual Eigen::Vector3f getPosition() const override;
    Eigen::Vector3f getDirection(const HitRecord& hitRecord, int area_light_row, int area_light_col) const;
private:
    Eigen::Vector3f p1p2;
    Eigen::Vector3f p2p3;
    double half_cell_len;
};


#endif //RAYTRACER_AREALIGHT_H
