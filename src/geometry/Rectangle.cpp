//
// Created by adrien on 11/03/23.
//

#include "Rectangle.h"

Rectangle::Rectangle(Eigen::Vector3f p1, Eigen::Vector3f p2, Eigen::Vector3f p3, Eigen::Vector3f p4, std::shared_ptr<Material> material)
: p1(p1), p2(p2), p3(p3), p4(p4), material(material) {
    Eigen::Vector3f v1 = p2 - p1;
    Eigen::Vector3f v2 = p3 - p1;
    normal = v1.cross(v2).normalized();
}

bool Rectangle::hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const {
    // todo: this is a plane hit
    float denom = normal.dot(ray.getDirection());

    if (std::abs(denom) < 1e-6)
        return false;

    float t = (p1 - ray.getOrigin()).dot(normal) / denom;

    if (t < 0)
        return false;

    hitRecord.t = t;
    hitRecord.point = ray.at(hitRecord.t);
    Eigen::Vector3f outwardNormal = normal;
    hitRecord.setFaceNormal(ray, outwardNormal);
    hitRecord.material = material;

    return true;
}
