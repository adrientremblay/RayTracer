//
// Created by adrien on 27/02/23.
//

#include "HittableList.h"

HittableList::HittableList() {

}

HittableList::HittableList(std::shared_ptr<Hittable> hittable) {
    add(hittable);
}

void HittableList::clear() {
    hittables.clear();
}

void HittableList::add(std::shared_ptr<Hittable> object) {
    hittables.push_back(object);
}

bool HittableList::hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const {
    HitRecord tempRec;
    bool hitAnything = false;
    double closestSoFar = tMax;

    for (const std::shared_ptr<Hittable>& hittable: hittables) {
        if (hittable->hit(ray, tMin, closestSoFar, tempRec)) {
            hitAnything = true;
            closestSoFar = tempRec.t;
            hitRecord = tempRec;
        }
    }

    return hitAnything;
}
