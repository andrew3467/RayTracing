//
// Created by Andrew Graser on 7/16/2026.
//


#pragma once

#include <memory>
#include <vector>

#include "Ray.h"
#include "Interval.h"
#include "util/AABB.h"


class Material;

struct HitRecord {
    Point3 Point;
    vec3 Normal;
    std::shared_ptr<Material> Material;

    double T;
    double U, V;
    bool FrontFace;

    void SetFaceNormal(const Ray& r, const vec3& outwardNormal) {
        FrontFace = dot(r.Direction(), outwardNormal) < 0;
        Normal = FrontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool Hit(const Ray& r, Interval interval, HitRecord& rec) const = 0;
    virtual AABB BoundingBox() const = 0;
};


class HittableList : public Hittable {
public:
    HittableList() = default;
    HittableList(std::shared_ptr<Hittable> object) { Add(object); }

    void clear() {Objects.clear();}

    void Add(std::shared_ptr<Hittable> object) {
        Objects.push_back(object);
        mBounds = AABB(mBounds, object->BoundingBox());
    }

    virtual bool Hit(const Ray& r, Interval interval, HitRecord& rec) const override {
        HitRecord record;
        bool hit = false;
        double closestHit = interval.Max;

        for(const auto& object : Objects) {
            if(object->Hit(r, Interval(interval.Min, closestHit), record)) {
                hit = true;
                closestHit = record.T;
                rec = record;
            }
        }

        return hit;
    }

    AABB BoundingBox() const override {return mBounds;}



public:
    std::vector<std::shared_ptr<Hittable>> Objects;

private:
    ///World bounding box
    AABB mBounds;
};