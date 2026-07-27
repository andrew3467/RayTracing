//
// Created by Andrew Graser on 7/27/2026.
//


#pragma once

#include "Hittable.h"
#include "Material.h"
#include "Texture.h"


class ConstantMedium : public Hittable {
public:
    ConstantMedium(std::shared_ptr<Hittable> boundary, double density, std::shared_ptr<Material>& mat)
    : mBoundary(boundary), mNegInvDensity(-1 / density), mPhaseFunc(mat) {}

    ConstantMedium(std::shared_ptr<Hittable> boundary, double density, const Color& albedo)
            : mBoundary(boundary), mNegInvDensity(-1 / density), mPhaseFunc(std::make_shared<Isotropic>(albedo)) {}

    virtual bool Hit(const Ray& r, Interval r_t, HitRecord& rec) const override {
        //Rec1 -> Ray enter
        //Rec2 -> Ray exit
        HitRecord rec1, rec2;

        if(!mBoundary->Hit(r, Interval::Universe, rec1)) return false;
        if(!mBoundary->Hit(r, Interval(rec1.T + 0.0001, INFINITY), rec2)) return false;

        if(rec1.T < r_t.Min) rec1.T = r_t.Min;
        if(rec2.T > r_t.Max) rec2.T = r_t.Max;

        if(rec1.T >= rec2.T) return false;

        if(rec1.T < 0) rec1.T = 0;

        auto rayLength = r.Direction().length();
        auto distanceInBoundary = (rec2.T - rec1.T) * rayLength;
        auto hitDist = mNegInvDensity * std::log(RandomDouble01());

        if(hitDist > distanceInBoundary) return false;

        rec.T = rec1.T + hitDist / rayLength;
        rec.Point = r.At(rec.T);

        rec.Normal = vec3(1.0, 0.0, 0.0);
        rec.FrontFace = true;
        rec.Material = mPhaseFunc;

        return true;
    }

    virtual AABB BoundingBox() const override {
        return mBoundary->BoundingBox();
    }

private:
    std::shared_ptr<Hittable> mBoundary;
    double mNegInvDensity;
    std::shared_ptr<Material> mPhaseFunc;
};
