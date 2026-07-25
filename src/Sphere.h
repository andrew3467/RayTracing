//
// Created by Andrew Graser on 7/16/2026.
//


#pragma once

#include "Hittable.h"


class Sphere : public Hittable {
public:
    Sphere(const Point3& center, double radius, const std::shared_ptr<Material>& mat)
    : mCenter(center, vec3(0)), mRadius(std::fmax(0, radius)), mMaterial(mat)
    {
        vec3 rvec(radius);
        mBounds = AABB(center - rvec, center + rvec);
    }

    Sphere(const Point3& point1, const Point3 point2, double radius, const std::shared_ptr<Material>& mat)
    : mCenter(point1, point2 - point1), mRadius(std::fmax(0, radius)), mMaterial(mat)
    {
        vec3 rvec(radius);
        AABB box1(mCenter.At(0) - rvec, mCenter.At(0) + rvec);
        AABB box2(mCenter.At(1) - rvec, mCenter.At(1) + rvec);
        mBounds = AABB(box1, box2);
    }



    bool Hit(const Ray& r, Interval interval, HitRecord& rec) const override {
        Point3 curCenter = mCenter.At(r.Time());
        vec3 oc = curCenter - r.Origin();
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

        vec3 outwardNormal = (rec.Point - curCenter) / mRadius;
        rec.SetFaceNormal(r, outwardNormal);
        GetSphereUV(outwardNormal, rec.U, rec.V);

        return true;
    }

    AABB BoundingBox() const override {
        return mBounds;
    }



private:
    double mRadius;
    Ray mCenter;
    std::shared_ptr<Material> mMaterial;

    AABB mBounds;


public:
    static void GetSphereUV(const Point3& p, double& u, double& v){
        double theta = std::acos(-p.y());
        double phi = std::atan2(-p.z(), p.x()) + PI;

        u = phi / (PI * 2);
        v = theta / PI;
    }
};