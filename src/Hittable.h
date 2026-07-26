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


class Translate : public Hittable {
public:
    Translate(std::shared_ptr<Hittable>& obj, const vec3& offset) : mObject(obj), mOffset(offset) {
        mBounds = mObject->BoundingBox() + mOffset;
    }

    bool Hit(const Ray& r, Interval r_t, HitRecord& rec) const override {
        Ray offsetR(r.Origin() - mOffset, r.Direction(), r.Time());

        if(!mObject->Hit(offsetR, r_t, rec))
            return false;

        rec.Point += mOffset;

        return true;
    }

    AABB BoundingBox() const override {return mBounds;}


private:
    std::shared_ptr<Hittable> mObject;
    vec3 mOffset;
    AABB mBounds;
};

class RotateY : public Hittable {
public:
    RotateY(std::shared_ptr<Hittable> object, double angle) : mObject(object) {
            auto radians = ToRadians(angle);
            mSinTheta = std::sin(radians);
            mCosTheta = std::cos(radians);
            mBounds = mObject->BoundingBox();

            Point3 min( INFINITY,  INFINITY,  INFINITY);
            Point3 max(-INFINITY, -INFINITY, -INFINITY);

            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 2; j++) {
                    for (int k = 0; k < 2; k++) {
                        auto x = i*mBounds.X.Max + (1-i)*mBounds.X.Min;
                        auto y = j*mBounds.Y.Max + (1-j)*mBounds.Y.Min;
                        auto z = k*mBounds.Z.Max + (1-k)*mBounds.Z.Min;

                        auto newx =  mCosTheta*x + mSinTheta*z;
                        auto newz = -mSinTheta*x + mCosTheta*z;

                        vec3 tester(newx, y, newz);

                        for (int c = 0; c < 3; c++) {
                            min[c] = std::fmin(min[c], tester[c]);
                            max[c] = std::fmax(max[c], tester[c]);
                        }
                    }
                }
            }

            mBounds = AABB(min, max);
    }
    bool Hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {

        // Transform the ray from world space to object space.

        auto origin = Point3(
                (mCosTheta * r.Origin().x()) - (mSinTheta * r.Origin().z()),
                r.Origin().y(),
                (mSinTheta * r.Origin().x()) + (mCosTheta * r.Origin().z())
        );

        auto direction = vec3(
                (mCosTheta * r.Direction().x()) - (mSinTheta * r.Direction().z()),
                r.Direction().y(),
                (mSinTheta * r.Direction().x()) + (mCosTheta * r.Direction().z())
        );

        Ray rotated_r(origin, direction, r.Time());

        // Determine whether an intersection exists in object space (and if so, where).

        if (!mObject->Hit(rotated_r, ray_t, rec))
            return false;

        // Transform the intersection from object space back to world space.

        rec.Point = Point3(
                (mCosTheta * rec.Point.x()) + (mSinTheta * rec.Point.z()),
                rec.Point.y(),
                (-mSinTheta * rec.Point.x()) + (mCosTheta * rec.Point.z())
        );

        rec.Normal= vec3(
                (mCosTheta * rec.Normal.x()) + (mSinTheta * rec.Normal.z()),
                rec.Normal.y(),
                (-mSinTheta * rec.Normal.x()) + (mCosTheta * rec.Normal.z())
        );

        return true;
    }

    AABB BoundingBox() const override { return mBounds; }

private:
    std::shared_ptr<Hittable> mObject;
    double mSinTheta;
    double mCosTheta;
    AABB mBounds;
};