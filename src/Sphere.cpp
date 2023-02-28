//
// Created by adrien on 27/02/23.
//

#include "Sphere.h"

Sphere::Sphere(Eigen::Vector3f center, double radius) : center(center), radius(radius) {

}


bool Sphere::hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const {
    Eigen::Vector3f oc = ray.getOrigin() - center;
    double a = ray.getDirection().squaredNorm(); // magnitude squared
    double half_b = oc.dot(ray.getDirection());
    double c = oc.squaredNorm() - radius*radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0)
        return false;

    double discriminantSqrt = sqrt(discriminant);
    double root = (-half_b - discriminantSqrt) / a;
    if (root < tMin || root > tMax) {
        root = (-half_b + discriminantSqrt) / a;
        if (root < tMin || root > tMax)
            return false;
    }

    hitRecord.t = root;
    hitRecord.point = ray.at(hitRecord.t);
    Eigen::Vector3f outwardNormal = (hitRecord.point - center) / radius; // wont be a unit vector??
    hitRecord.setFaceNormal(ray, outwardNormal);

    return true;
}
