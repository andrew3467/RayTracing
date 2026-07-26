//
// Created by Andrew Graser on 7/24/2026.
//


#pragma once

#include "../Interval.h"
#include "../Ray.h"
#include "vec3.h"


class AABB {
public:
    AABB() = default;

    AABB(const Interval& x, const Interval& y, const Interval& z)
        : X(x), Y(y), Z(z) {
        PadToMin();
    }

    AABB(const Point3 a, const Point3 b) {
        X = (a[0] <= b[0]) ? Interval(a[0], b[0]) : Interval(b[0], a[0]);
        Y = (a[1] <= b[1]) ? Interval(a[1], b[1]) : Interval(b[1], a[1]);
        Z = (a[2] <= b[2]) ? Interval(a[2], b[2]) : Interval(b[2], a[2]);

        PadToMin();
    }

    AABB(const AABB& a, const AABB& b) {
        X = Interval(a.X, b.X);
        Y = Interval(a.Y, b.Y);
        Z = Interval(a.Z, b.Z);

        PadToMin();
    }



    const Interval& AxisInterval(int n) const {
        if(n == 1) return Y;
        if(n == 2) return Z;
        return X;
    }

    bool Hit(const Ray& r, Interval r_t) const {
        const Point3 &rayOrigin = r.Origin();
        const vec3 &rayDir = r.Direction();

        for (int axis = 0; axis < 3; ++axis) {
            const Interval &ax = AxisInterval(axis);
            const double adinv = 1.0 / rayDir[axis];

            double t0 = (ax.Min - rayOrigin[axis]) * adinv;
            double t1 = (ax.Max - rayOrigin[axis]) * adinv;

            if (t0 < t1) {
                if (t0 > r_t.Min) r_t.Min = t0;
                if (t1 < r_t.Max) r_t.Max = t1;
            } else {
                if (t1 > r_t.Min) r_t.Min = t1;
                if (t0 < r_t.Max) r_t.Max = t0;
            }

            if (r_t.Max <= r_t.Min)
                return false;
        }
        return true;
    }

    int LongestAxis() const {
        if(X.Size() > Y.Size())
            return X.Size() > Z.Size() ? 0 : 2;
        else
            return Y.Size() > Z.Size() ? 1 : 2;
    }


private:
    void PadToMin() {
        double delta = 0.0001;

        if(X.Size() < delta) X = X.Expand(delta);
        if(Y.Size() < delta) Y = Y.Expand(delta);
        if(Z.Size() < delta) Z = Z.Expand(delta);
    }


public:
    static const AABB Empty, Universe;

public:
    Interval X, Y, Z;
};


const AABB AABB::Empty    = AABB(Interval::Empty,    Interval::Empty,    Interval::Empty);
const AABB AABB::Universe = AABB(Interval::Universe, Interval::Universe, Interval::Universe);


AABB operator+(const AABB& bound, const vec3& offset) {
    return {
            bound.X + offset.x(),
            bound.Y + offset.y(),
            bound.Z + offset.z(),
    };
}

AABB operator+(const vec3& offset, const AABB& bound) {
    return {
            bound.X + offset.x(),
            bound.Y + offset.y(),
            bound.Z + offset.z(),
    };
}