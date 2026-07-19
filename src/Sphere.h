//
// Created by Andrew Graser on 7/16/2026.
//


#pragma once

#include "Hittable.h"


class Sphere : public Hittable {
public:
    Sphere(const Point3& center, double radius, const std::shared_ptr<Material>& mat) : mCenter(center), mRadius(std::fmax(0, radius)), mMaterial(mat) {}

    bool Hit(const Ray& r, Interval interval, HitRecord& rec) const override {
        vec3 oc = mCenter - r.Origin();
        double a = r.Direction().LengthSquared();
        double h = dot(r.Direction(), oc);
        double c = oc.LengthSquared() - mRadius * mRadius;
        double discriminant = h*h - a*c;

        if(discriminant < 0) {
            return false;
        }

        //Find root in acceptable range
        double sqrtd = std::sqrt(discriminant);
        double root = (h - sqrtd) / a;
        if(!interval.Surrounds(root)) {
            root = (h + sqrtd) / a;
            if(!interval.Surrounds(root)) {
                return false;
            }
        }

        rec.T = root;
        rec.Point = r.At(rec.T);
        rec.Material = mMaterial;

        vec3 outwardNormal = (rec.Point - mCenter) / mRadius;
        rec.SetFaceNormal(r, outwardNormal);

        return true;
    }



private:
    double mRadius;
    Point3 mCenter;
    std::shared_ptr<Material> mMaterial;
};