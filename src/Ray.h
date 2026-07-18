//
// Created by Andrew Graser on 7/16/2026.
//


#pragma once


#include "vec3.h"


class Ray {
public:
    Ray() = default;

    Ray(const Point3& origin, const vec3& direction) : mOrigin(origin), mDirection(direction) {}

    const Point3& Origin() const {return mOrigin;}
    const vec3& Direction() const {return mDirection;}

    Point3 At(double t) const {
        return mOrigin + t * mDirection;
    }


private:
    Point3 mOrigin;
    vec3 mDirection;
};