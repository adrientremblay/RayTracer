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
    // Checking if inside plane
    float denom = normal.dot(ray.getDirection());
    if (std::abs(denom) < 1e-6)
        return false;
    float t = (p1 - ray.getOrigin()).dot(normal) / denom;
    if (t < 0)
        return false;

    // Checking if inside rectangle
    Eigen::Vector3f point_on_plane = ray.at(t);
    Eigen::Vector3f v = point_on_plane - p1;

    float alpha = v.dot(p2 - p1) / (p2-p1).dot(p2 - p1);
    float beta = v.dot(p4 - p1) / (p4-p1).dot(p4 - p1);

    if (!(alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1))
        return false;

    hitRecord.t = t;
    hitRecord.point = ray.at(hitRecord.t);
    Eigen::Vector3f outwardNormal = normal;
    hitRecord.setFaceNormal(ray, outwardNormal);
    hitRecord.material = material;

    return true;
}
