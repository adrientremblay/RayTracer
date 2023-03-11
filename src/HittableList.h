//
// Created by adrien on 27/02/23.
//

#ifndef RAYTRACER_HITTABLELIST_H
#define RAYTRACER_HITTABLELIST_H

#include "geometry/Hittable.h"
#include <memory>
#include <vector>

class HittableList : public Hittable {
public:
    std::vector<std::shared_ptr<Hittable>> hittables;

    HittableList();
    HittableList(std::shared_ptr<Hittable> hittable);

    void clear();
    void add(std::shared_ptr<Hittable> object);

    virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& hitRecord) const override;
};


#endif //RAYTRACER_HITTABLELIST_H
