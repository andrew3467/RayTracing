//
// Created by Andrew Graser on 7/16/2026.
//


#pragma once


#include "util/vec3.h"


class Ray {
public:
    Ray() = default;

    Ray(const Point3& origin, const vec3& direction) : mOrigin(origin), mDirection(direction), mTime(0) {}
    Ray(const Point3& origin, const vec3& direction, double time) : mOrigin(origin), mDirection(direction), mTime(time) {}

    const Point3& Origin() const {return mOrigin;}
    const vec3& Direction() const {return mDirection;}
    const double Time() const {return mTime;}


    Point3 At(double t) const {
        return mOrigin + t * mDirection;
    }


private:
    Point3 mOrigin;
    vec3 mDirection;
    double mTime;
};