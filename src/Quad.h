//
// Created by Andrew Graser on 7/24/2026.
//


#pragma once

#include <utility>

#include "Hittable.h"


class Quad : public Hittable {
public:
    Quad(const Point3& Q, const vec3& u, const vec3& v, std::shared_ptr<Material> mat)
        : Q(Q), u(u), v(v), mMaterial(std::move(mat)) {

        auto n = cross(u, v);
        Normal = UnitVector(n);
        D = dot(Normal, Q);
        W = n / dot(n,n);

        SetBounds();
    }

    virtual void SetBounds() {
        auto diag1 = AABB(Q, Q + u + v);
        auto diag2 = AABB(Q + u, Q + v);
        mBounds = AABB(diag1, diag2);
    }

    AABB BoundingBox() const override {return mBounds;}

    bool Hit(const Ray& r, Interval r_t, HitRecord& rec) const override {
        double denom = dot(Normal, r.Direction());

        if(std::fabs(denom) < 1e-8) return false;

        auto t = (D - dot(Normal, r.Origin())) / denom;
        if(!r_t.Contains(t)) return false;

        auto intersection = r.At(t);
        vec3 planarHitPtVector = intersection - Q;
        auto alpha = dot(W, cross(planarHitPtVector, v));
        auto beta = dot(W, cross(u, planarHitPtVector));

        if(!IsInterior(alpha, beta, rec)) return false;

        rec.T = t;
        rec.Point = intersection;
        rec.Material = mMaterial;
        rec.SetFaceNormal(r, Normal);

        return true;
    }

    virtual bool IsInterior(double a, double b, HitRecord& rec) const {
        Interval unitInterval = Interval(0,1);

        if(!unitInterval.Contains(a) || !unitInterval.Contains(b)) return false;

        rec.U = a;
        rec.V = b;
        return true;
    }


private:
    Point3 Q;
    vec3 u, v;
    vec3 W;

    std::shared_ptr<Material> mMaterial;
    AABB mBounds;

    vec3 Normal;
    double D;
};



inline std::shared_ptr<HittableList> Box(const Point3& a, const Point3& b, std::shared_ptr<Material> mat) {
    auto sides = std::make_shared<HittableList>();

    auto min = Min(a, b);
    auto max = Max(a, b);

    auto dx = vec3(max.x() - min.x(), 0, 0);
    auto dy = vec3(0, max.y() - min.y(), 0);
    auto dz = vec3(0, 0, max.z() - min.z());

    sides->Add(std::make_shared<Quad>(Point3(min.x(), min.y(), max.z()),  dx,  dy, mat)); // front
    sides->Add(std::make_shared<Quad>(Point3(max.x(), min.y(), max.z()), -dz,  dy, mat)); // right
    sides->Add(std::make_shared<Quad>(Point3(max.x(), min.y(), min.z()), -dx,  dy, mat)); // back
    sides->Add(std::make_shared<Quad>(Point3(min.x(), min.y(), min.z()),  dz,  dy, mat)); // left
    sides->Add(std::make_shared<Quad>(Point3(min.x(), max.y(), max.z()),  dx, -dz, mat)); // top
    sides->Add(std::make_shared<Quad>(Point3(min.x(), min.y(), min.z()),  dx,  dz, mat)); // bottom

    return sides;
}